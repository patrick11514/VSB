#pragma once

#include <exception>
#include <vector>
#include <memory>

class ArrayException : public std::exception
{
public:
    explicit ArrayException(const char *message) : message(message) {}

    const char *what() const noexcept override
    {
        return this->message;
    }

private:
    const char *message;
};

// TODO: implement NumpyArray

template <typename BeginIt, typename EndIt>
class Iterable;

template <typename T>
class NumpyArray
{
    T *data;
    size_t size;

public:
    NumpyArray(std::vector<T> data)
    {
        this->size = data.size();
        this->data = new T[this->size];
        std::copy(data.begin(), data.end(), this->data);
    }

    ~NumpyArray()
    {
        delete[] this->data;
    }

    T &operator[](size_t index)
    {
        if (index >= this->size)
        {
            throw ArrayException("Index out of bound");
        }
        return this->data[index];
    }

    const T &operator[](size_t index) const
    {
        if (index >= this->size)
        {
            throw ArrayException("Index out of bound");
        }

        return this->data[index];
    }

    class NumpyIterator
    {
        T *data;

    public:
        NumpyIterator(T *data) : data(data) {}

        T operator*() const
        {
            return *data;
        }

        bool operator==(const NumpyIterator &other) const
        {
            return this->data == other.data;
        }

        bool operator!=(const NumpyIterator &other) const
        {
            return !this->operator==(other);
        }

        NumpyIterator &operator++()
        {
            ++this->data;
            return *this;
        }

        NumpyIterator operator++(int)
        {
            NumpyIterator pre = *this;
            ++this->data;
            return pre;
        }

        NumpyIterator &operator--()
        {
            --this->data;
            return *this;
        }

        NumpyIterator operator--(int)
        {
            NumpyIterator pre = *this;
            --this->data;
            return pre;
        }

        NumpyIterator &operator+=(size_t value)
        {
            this->data += value;
            return *this;
        }

        NumpyIterator &operator-=(size_t value)
        {
            this->data -= value;
            return *this;
        }

        NumpyIterator &operator+(size_t value)
        {
            this->data = this->data + value;
            return *this;
        }

        NumpyIterator &operator-(size_t value)
        {
            this->data = this->data - value;
            return *this;
        }
    };

    NumpyIterator
    begin()
    {
        return NumpyIterator(this->data);
    }

    const NumpyIterator
    begin() const
    {
        return NumpyIterator(this->data);
    }

    NumpyIterator
    end()
    {
        return NumpyIterator(this->data + this->size);
    }

    const NumpyIterator
    end() const
    {
        return NumpyIterator(this->data + this->size);
    }

    class NumpySlice
    {
        T *data;
        size_t index;
        size_t dataStart;
        size_t dataEnd;

    public:
        NumpySlice(T *data, size_t start, size_t end) : data(data), index(0), dataStart(start), dataEnd(end) {}

        size_t size() const
        {
            return this->dataEnd - this->dataStart;
        }

        const T &operator[](size_t index) const
        {
            size_t range = this->dataEnd - this->dataStart;
            if (index > range)
            {
                throw ArrayException("Index out of bounds");
            }

            return this->data[this->dataStart + index];
        }

        NumpySlice slice() const
        {
            size_t range = this->dataEnd - this->dataStart;

            return NumpySlice(this->data + this->dataStart, 0, range);
        }

        NumpySlice slice(size_t start) const
        {
            size_t range = this->dataEnd - this->dataStart;

            if (start > range)
            {
                throw ArrayException("Slice cannot start after end of original slice");
            }

            return NumpySlice(this->data + this->dataStart, start, range);
        }

        NumpySlice slice(size_t start, size_t end) const
        {
            size_t range = this->dataEnd - this->dataStart;

            if (start > range)
            {
                throw ArrayException("Slice cannot start after end of original slice");
            }

            if (end > range)
            {
                throw ArrayException("Slice cannot end after end of original slice");
            }

            if (start > end)
            {
                throw ArrayException("Start cannot be bigger than end");
            }

            return NumpySlice(this->data + this->dataStart, start, end);
        }

        class SliceIterator
        {
            T *data;

        public:
            SliceIterator(T *data) : data(data) {}

            T operator*() const
            {
                return *data;
            }

            bool operator==(const SliceIterator &other) const
            {
                return this->data == other.data;
            }

            bool operator!=(const SliceIterator &other) const
            {
                return !this->operator==(other);
            }

            SliceIterator &operator++()
            {
                ++this->data;
                return *this;
            }

            SliceIterator operator++(int)
            {
                NumpyIterator pre = *this;
                ++this->data;
                return pre;
            }

            SliceIterator &operator--()
            {
                --this->data;
                return *this;
            }

            SliceIterator operator--(int)
            {
                NumpyIterator pre = *this;
                --this->data;
                return pre;
            }

            SliceIterator &operator+=(size_t value)
            {
                this->data += value;
                return *this;
            }

            SliceIterator &operator-=(size_t value)
            {
                this->data -= value;
                return *this;
            }

            SliceIterator &operator+(size_t value)
            {
                this->data = this->data + value;
                return *this;
            }

            SliceIterator &operator-(size_t value)
            {
                this->data = this->data - value;
                return *this;
            }
        };

        SliceIterator begin()
        {
            return SliceIterator(this->data + this->dataStart);
        }

        const SliceIterator begin() const
        {
            return SliceIterator(this->data + this->dataStart);
        }

        SliceIterator end()
        {
            return SliceIterator(this->data + this->dataEnd);
        }

        const SliceIterator end() const
        {
            return SliceIterator(this->data + this->dataEnd);
        }

        friend std::ostream &operator<<(std::ostream &stream, const NumpySlice &slice)
        {
            for (size_t i = slice.dataStart; i < slice.dataEnd; ++i)
            {
                if (i > slice.dataStart)
                {
                    stream << ", ";
                }
                stream << slice.data[i];
            }

            return stream;
        }

        NumpyArray<T> operator+(int number) const
        {
            size_t range = this->dataEnd - this->dataStart;

            std::vector<T> newData;
            newData.reserve(range);

            for (size_t i = this->dataStart; i < this->dataEnd; ++i)
            {
                newData.emplace_back(this->data[i] + number);
            }

            return NumpyArray(newData);
        }

        friend NumpyArray<T> operator+(int number, const NumpySlice &slice)
        {
            size_t range = slice.dataEnd - slice.dataStart;

            std::vector<T> newData;
            newData.reserve(range);

            for (size_t i = slice.dataStart; i < slice.dataEnd; ++i)
            {
                newData.emplace_back(slice.data[i] + number);
            }

            return NumpyArray(newData);
        }

        NumpyArray<T> operator+(const NumpySlice &slice) const
        {
            size_t range = this->dataEnd - this->dataStart;
            size_t otherRange = slice.dataEnd - slice.dataStart;

            if (range != otherRange)
            {
                if (range == 1)
                {
                    return slice.operator+(this->operator[](0));
                }

                if (otherRange == 1)
                {
                    return this->operator+(slice[0]);
                }

                throw ArrayException("Slices must have same size");
            }

            std::vector<T> newData;
            newData.reserve(range);

            for (size_t i = 0; i < range; ++i)
            {
                newData.emplace_back(this->data[this->dataStart + i] + slice.data[slice.dataStart + i]);
            }

            return NumpyArray(newData);
        }

        NumpyArray<T> operator+(const NumpyArray &array) const
        {
            size_t range = this->dataEnd - this->dataStart;

            if (range != array.size)
            {
                if (range == 1)
                {
                    return array.operator+(this->operator[](0));
                }

                if (array.size == 1)
                {
                    return this->operator+(array[0]);
                }
                throw ArrayException("Slice's and Array's size must be same");
            }

            std::vector<T> newData;
            newData.reserve(range);

            for (size_t i = 0; i < range; ++i)
            {
                newData.emplace_back(this->data[this->dataStart + i] + array[i]);
            }

            return NumpyArray(newData);
        }

        NumpyArray<T> operator*(int number) const
        {
            size_t range = this->dataEnd - this->dataStart;

            std::vector<T> newData;
            newData.reserve(range);

            for (size_t i = this->dataStart; i < this->dataEnd; ++i)
            {
                newData.emplace_back(this->data[i] * number);
            }

            return NumpyArray(newData);
        }

        friend NumpyArray<T> operator*(int number, const NumpySlice &slice)
        {
            size_t range = slice.dataEnd - slice.dataStart;

            std::vector<T> newData;
            newData.reserve(range);

            for (size_t i = slice.dataStart; i < slice.dataEnd; ++i)
            {
                newData.emplace_back(slice.data[i] * number);
            }

            return NumpyArray(newData);
        }

        NumpyArray<T> operator*(const NumpySlice &slice) const
        {
            size_t range = this->dataEnd - this->dataStart;
            size_t otherRange = slice.dataEnd - slice.dataStart;

            if (range != otherRange)
            {
                if (range == 1)
                {
                    return slice.operator*(this->operator[](0));
                }

                if (otherRange == 1)
                {
                    return this->operator*(slice[0]);
                }

                throw ArrayException("Slices must have same size");
            }

            std::vector<T> newData;
            newData.reserve(range);

            for (size_t i = 0; i < range; ++i)
            {
                newData.emplace_back(this->data[this->dataStart + i] * slice.data[slice.dataStart + i]);
            }

            return NumpyArray(newData);
        }

        NumpyArray<T> operator*(const NumpyArray &array) const
        {
            size_t range = this->dataEnd - this->dataStart;

            if (range != array.size)
            {
                if (range == 1)
                {
                    return array.operator*(this->operator[](0));
                }

                if (array.size == 1)
                {
                    return this->operator*(array[0]);
                }
                throw ArrayException("Slice's and Array's size must be same");
            }

            std::vector<T> newData;
            newData.reserve(range);

            for (size_t i = 0; i < range; ++i)
            {
                newData.emplace_back(this->data[this->dataStart + i] * array[i]);
            }

            return NumpyArray(newData);
        }

        Iterable<SliceIterator, SliceIterator> itter()
        {
            return Iterable(this->begin(), this->end());
        }
    };

    NumpySlice slice() const
    {
        return NumpySlice(this->data, 0, this->size);
    }

    NumpySlice slice(size_t start) const
    {
        if (start > this->size)
        {
            throw ArrayException("Slice cannot start after end of original array");
        }

        return NumpySlice(this->data, start, this->size);
    }

    NumpySlice slice(size_t start, size_t end) const
    {
        if (start > this->size)
        {
            throw ArrayException("Slice cannot start after end of original array");
        }

        if (end > this->size)
        {
            throw ArrayException("Slice cannot end after end of original array");
        }

        if (start > end)
        {
            throw ArrayException("Start cannot be bigger than end");
        }

        return NumpySlice(this->data, start, end);
    }

    friend std::ostream &operator<<(std::ostream &stream, const NumpyArray &array)
    {
        for (size_t i = 0; i < array.size; ++i)
        {
            if (i > 0)
            {
                stream << ", ";
            }
            stream << array.data[i];
        }

        return stream;
    }

    NumpyArray<T> operator+(int number) const
    {

        std::vector<T> newData;
        newData.reserve(this->size);

        for (size_t i = 0; i < this->size; ++i)
        {
            newData.emplace_back(this->data[i] + number);
        }

        return NumpyArray(newData);
    }

    friend NumpyArray<T> operator+(int number, NumpyArray<T> &data)
    {

        std::vector<T> newData;
        newData.reserve(data.size);

        for (size_t i = 0; i < data.size; ++i)
        {
            newData.emplace_back(data.data[i] + number);
        }

        return NumpyArray(newData);
    }

    NumpyArray<T> operator+(const NumpyArray<T> &other) const
    {
        if (this->size != other.size)
        {
            if (this->size == 1)
            {
                return other.operator+(this->data[0]);
            }

            if (other.size == 1)
            {
                return this->operator+(other[0]);
            }

            throw ArrayException("Arrays must have same size");
        }

        std::vector<T> newData;
        newData.reserve(this->size);

        for (size_t i = 0; i < this->size; ++i)
        {
            newData.emplace_back(this->data[i] + other.data[i]);
        }

        return NumpyArray(newData);
    }

    NumpyArray<T> operator+(const NumpySlice &other) const
    {
        if (this->size != other.size())
        {
            if (this->size == 1)
            {
                return other.operator+(this->data[0]);
            }

            if (other.size() == 1)
            {
                return this->operator+(other[0]);
            }

            throw ArrayException("Array's and Slice's size must be same");
        }

        std::vector<T> newData;
        newData.reserve(this->size);

        for (size_t i = 0; i < this->size; ++i)
        {
            newData.emplace_back(this->data[i] + other[i]);
        }

        return NumpyArray(newData);
    }

    NumpyArray<T> operator*(int number) const
    {

        std::vector<T> newData;
        newData.reserve(this->size);

        for (size_t i = 0; i < this->size; ++i)
        {
            newData.emplace_back(this->data[i] * number);
        }

        return NumpyArray(newData);
    }

    friend NumpyArray<T> operator*(int number, NumpyArray<T> &data)
    {

        std::vector<T> newData;
        newData.reserve(data.size);

        for (size_t i = 0; i < data.size; ++i)
        {
            newData.emplace_back(data.data[i] * number);
        }

        return NumpyArray(newData);
    }

    NumpyArray<T> operator*(const NumpyArray<T> &other) const
    {
        if (this->size != other.size)
        {
            if (this->size == 1)
            {
                return other.operator*(this->data[0]);
            }

            if (other.size == 1)
            {
                return this->operator*(other[0]);
            }

            throw ArrayException("Arrays must have same size");
        }

        std::vector<T> newData;
        newData.reserve(this->size);

        for (size_t i = 0; i < this->size; ++i)
        {
            newData.emplace_back(this->data[i] * other.data[i]);
        }

        return NumpyArray(newData);
    }

    NumpyArray<T> operator*(const NumpySlice &other) const
    {
        if (this->size != other.size())
        {
            if (this->size == 1)
            {
                return other.operator*(this->data[0]);
            }

            if (other.size() == 1)
            {
                return this->operator*(other[0]);
            }

            throw ArrayException("Array's and Slice's size must be same");
        }

        std::vector<T> newData;
        newData.reserve(this->size);

        for (size_t i = 0; i < this->size; ++i)
        {
            newData.emplace_back(this->data[i] * other[i]);
        }

        return NumpyArray(newData);
    }

    Iterable<NumpyIterator, NumpyIterator> iter()
    {
        return Iterable(this->begin(), this->end());
    }
};

template <typename BeginIt, typename EndIt>
class Iterable
{
    BeginIt beginIt;
    EndIt endIt;

public:
    Iterable(BeginIt begin, EndIt end) : beginIt(begin), endIt(end) {}

    BeginIt begin()
    {
        return this->beginIt;
    }

    BeginIt begin() const
    {
        return this->beginIt;
    }

    EndIt end()
    {
        return this->endIt;
    }

    EndIt end() const
    {
        return this->endIt;
    }

    template <typename FunctionType, typename MBeginIt, typename MEndIt>
    class Mapping;

    template <typename FunctionType, typename FBeginIt, typename FEndIt>
    class Filtering
    {
        FBeginIt beginIt;
        FEndIt endIt;
        FunctionType predicate;

    public:
        Filtering(FBeginIt begin, FEndIt end, FunctionType predicate) : beginIt(begin), endIt(end), predicate(predicate)
        {
            auto currentBegin = this->beginIt;

            // move begin to first valid element
            while (currentBegin != this->endIt && !this->predicate(*currentBegin))
            {
                ++currentBegin;
            }

            this->beginIt = currentBegin;
        };

        class FilteringIterator
        {
            FBeginIt current;
            FEndIt endIt;
            FunctionType predicate;

        public:
            // ten predicate mi nešel předat jako reference :/
            // ale vůbec netuším, jestli by se to dalo nějak obejít
            FilteringIterator(FBeginIt current, FEndIt end, FunctionType predicate) : current(current), endIt(end), predicate(predicate) {}

            auto operator*() const
            {
                return *current;
            }

            bool operator==(const FilteringIterator &other) const
            {
                return this->current == other.current;
            }

            bool operator!=(const FilteringIterator &other) const
            {
                return !this->operator==(other);
            }

            FilteringIterator &operator++()
            {
                do
                {
                    ++this->current;
                } while (this->current != this->endIt && !this->predicate(*this->current));

                return *this;
            }

            FilteringIterator operator++(int)
            {
                FilteringIterator pre = *this;
                this->operator++();
                return pre;
            }
        };

        FilteringIterator begin()
        {
            return FilteringIterator(this->beginIt, this->endIt, this->predicate);
        }

        FilteringIterator begin() const
        {
            return FilteringIterator(this->beginIt, this->endIt, this->predicate);
        }

        FilteringIterator end()
        {
            return FilteringIterator(this->endIt, this->endIt, this->predicate);
        }

        FilteringIterator end() const
        {
            return FilteringIterator(this->endIt, this->endIt, this->predicate);
        }

        template <typename NewFunctionType>
        auto filter(NewFunctionType newPredicate)
        {
            return Filtering<NewFunctionType, FilteringIterator, FilteringIterator>(this->begin(), this->end(), newPredicate);
        }

        template <typename NewFunctionType>
        auto map(NewFunctionType mappingFunction)
        {
            return Mapping<NewFunctionType, FilteringIterator, FilteringIterator>(this->begin(), this->end(), mappingFunction);
        }

        auto reduce(auto reducer)
        {
            auto it = this->begin();
            auto endIt = this->end();
            auto acc = *it;
            it++;
            while (it != endIt)
            {
                reducer(acc, *it);
                ++it;
            };

            return acc;
        }
    };

    template <typename FunctionType>
    Filtering<FunctionType, BeginIt, EndIt> filter(FunctionType predicate)
    {
        return Filtering(this->beginIt, this->endIt, predicate);
    }

    template <typename FunctionType, typename MBeginIt, typename MEndIt>
    class Mapping
    {
        MBeginIt beginIt;
        MEndIt endIt;
        FunctionType mappingFunction;

    public:
        Mapping(MBeginIt beginIt, MEndIt endIt, FunctionType mappingFunction) : beginIt(beginIt), endIt(endIt), mappingFunction(mappingFunction) {}

        class MappingIterator
        {
            MBeginIt current;
            MEndIt endIt;
            FunctionType mappingFunction;

        public:
            MappingIterator(MBeginIt current, MEndIt end, FunctionType mappingFunction) : current(current), endIt(end), mappingFunction(mappingFunction) {}

            auto operator*() const
            {
                return this->mappingFunction(*current);
            }

            bool operator==(const MappingIterator &other) const
            {
                return this->current == other.current;
            }

            bool operator!=(const MappingIterator &other) const
            {
                return !this->operator==(other);
            }

            MappingIterator &operator++()
            {
                ++this->current;

                return *this;
            }

            MappingIterator operator++(int)
            {
                MappingIterator pre = *this;
                this->operator++();
                return pre;
            }
        };

        MappingIterator begin()
        {
            return MappingIterator(this->beginIt, this->endIt, this->mappingFunction);
        }

        MappingIterator begin() const
        {
            return MappingIterator(this->beginIt, this->endIt, this->mappingFunction);
        }

        MappingIterator end()
        {
            return MappingIterator(this->endIt, this->endIt, this->mappingFunction);
        }

        MappingIterator end() const
        {
            return MappingIterator(this->endIt, this->endIt, this->mappingFunction);
        }

        template <typename NewFunctionType>
        auto filter(NewFunctionType newPredicate)
        {
            return Filtering<NewFunctionType, MappingIterator, MappingIterator>(this->begin(), this->end(), newPredicate);
        }

        template <typename NewFunctionType>
        auto map(NewFunctionType mappingFunction)
        {
            return Mapping<NewFunctionType, MappingIterator, MappingIterator>(this->begin(), this->end(), mappingFunction);
        }

        auto reduce(auto reducer)
        {
            auto it = this->begin();
            auto endIt = this->end();
            auto acc = *it;
            it++;
            while (it != endIt)
            {
                reducer(acc, *it);
                ++it;
            };

            return acc;
        }
    };

    template <typename FunctionType>
    Mapping<FunctionType, BeginIt, EndIt> map(FunctionType mappingFunction)
    {
        return Mapping(this->beginIt, this->endIt, mappingFunction);
    }

    auto reduce(auto reducer)
    {
        auto it = this->beginIt;
        auto acc = *it;
        it++;
        while (it != this->endIt)
        {
            reducer(acc, *it);
            ++it;
        };

        return acc;
    }
};