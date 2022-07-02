#if !defined( ALLOC_HPP ) 
#define ALLOC_HPP
#include <iostream>
#include <memory>
#include <cstdio>
#include <map>
using byte = unsigned char;

class BlockItem {
private:
    BlockItem *next_ = nullptr;
public:
    BlockItem *getNext() {return next_;}
    void setNext(BlockItem *next) {this->next_ = next;}
};

class BlockCollection {
private:
    [[nodiscard]] byte *getStartOfSector() const {return startOfSector_;}
    size_t totalSize_;
    size_t itemSize_;
    BlockItem *last_ = nullptr;
    byte *startOfSector_;
public:
    BlockCollection()
        : totalSize_(0), itemSize_(0), startOfSector_(nullptr) 
    {}
    BlockCollection(byte *startOfSector, size_t itemSize, size_t totalSize)
        : totalSize_(totalSize), itemSize_(itemSize), startOfSector_(startOfSector) 
    {}
    bool operator<(const BlockCollection &other) const {return this->itemSize_ < other.itemSize_;}
    void put(BlockItem *item) {
        if (item == nullptr) throw std::logic_error("New block item shouldn't be nullptr");
        if (reinterpret_cast<byte *>(item) - startOfSector_ < 0 || reinterpret_cast<byte *>(item) - startOfSector_ >= (long long) totalSize_) {
            throw std::logic_error("This item doesn't belong to this collection " + std::to_string(reinterpret_cast<byte *>(item) - startOfSector_));
        }
        item->setNext(last_);
        last_ = item;
    }
    BlockItem *pop() {
        if (last_ == nullptr) throw std::logic_error("Block collection is empty");
        auto result = last_;
        last_ = last_->getNext();
        return result;
    }
    friend class Allocator;
};

class Allocator {
private:
    std::map<size_t, BlockCollection> blocks;
public:
    void *allocate(size_t n) {
        for (auto &elem: blocks) {
            if (elem.second.itemSize_ >= n) {
                try {
                    std::cout << "trying to give block size: " << elem.second.itemSize_ << std::endl;
                    return reinterpret_cast<void *>(elem.second.pop());
                } catch (const std::logic_error &e) {
                    continue;
                } catch (...) {throw;}
            }
        }
        throw std::bad_alloc();
    }
    void deallocate(BlockItem *pointer) {
        for (auto &elem: blocks) {
            try {
                elem.second.put(pointer);
                return;
            } catch (const std::logic_error &e) {
                continue;
            } catch (...) {throw;}
        }
    }
    ~Allocator() {
        for (const auto &elem: blocks) {delete[] elem.second.getStartOfSector();}
    }
    Allocator(const std::map<size_t, size_t> &parameters, size_t sizeofType) {
        for (const auto &elem: parameters) {
            auto key = elem.first;
            auto value = elem.second;
            blocks[key] = BlockCollection(new byte[sizeofType * key * value], sizeofType * key, sizeofType * key * value);
            for (size_t i = 0; i != value; ++i) {
                std::cout << i << " " << sizeofType * key << std::endl;
                blocks[key].put(new(blocks[key].getStartOfSector() + sizeofType * key * i) BlockItem);
            }
        }
    }
    class Builder {
    private:
        std::map<size_t, size_t> parameters_;
        size_t sizeOfType_ = 0;
    public:
        Builder &setBlocks(size_t sizeOfBlock, size_t countOfBlock) {
            parameters_[sizeOfBlock] = countOfBlock;
            return *this;
        }
        Builder &setType(size_t sizeOfType) {
            sizeOfType_ = sizeOfType;
            return *this;
        }
        std::shared_ptr<Allocator> build() {
            if (sizeOfType_ == 0 || parameters_.empty()) {
                throw std::logic_error("Incorrect build process");
            }
            return std::make_shared<Allocator>(parameters_, sizeOfType_);
        }
    };
};

template<typename T> class AllocatorWrapper {
private:
    std::shared_ptr<Allocator> innerAlloc;
public:
    using value_type = T;
    template<typename U> friend class AllocatorWrapper;

    explicit AllocatorWrapper(const std::map<size_t, size_t>& parameters) {
        auto innerBuilder = Allocator::Builder();
        for (const auto& elem: parameters) {innerBuilder = innerBuilder.setBlocks(elem.first * sizeof (T), elem.second);}
        innerAlloc = innerBuilder.setType(sizeof (T)).build();
    }

    template<typename U> explicit AllocatorWrapper<T>(const AllocatorWrapper<U>& other)
        : innerAlloc(other.innerAlloc) 
    {}

    template<typename U> AllocatorWrapper<T>& operator=(const AllocatorWrapper<U>& other) {
        if (this == &other) return *this;
        this->innerAlloc = other.innerAlloc;
        return *this;
    }
    class Builder {
    private:
        std::map<size_t, size_t> parameters_;
    public:
        Builder& setBlocks(size_t sizeOfBlock, size_t countOfBlock) {
            parameters_[sizeOfBlock] = countOfBlock;
            return *this;
        }
        AllocatorWrapper<T> build() {
            return AllocatorWrapper<T>(parameters_);
        }
    };
    T* allocate(size_t n) {return reinterpret_cast<T *>(innerAlloc->allocate(n * sizeof(T)));}
    void deallocate(T* p, size_t n) {innerAlloc->deallocate(reinterpret_cast<BlockItem*>(p));}
};
#endif