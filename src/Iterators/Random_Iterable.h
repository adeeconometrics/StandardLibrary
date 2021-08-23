#pragma once

template <typename Container> class Random_Iterable{
    public:
        typedef Container value_type;
        typedef value_type* pointer_type;
        typedef value_type& reference_type;

    public:
        Random_Iterable() = default;
        Random_Iterable(pointer_type ptr): m_ptr(ptr){}
        Random_Iterable(const Container& other) = delete;
        Random_Iterable(Container&& other) = delete;

        virtual ~Random_Iterable() = default;

        Random_Iterable<Container>& operator=(const Random_Iterable<Container>& other) = delete;
        Random_Iterable<Container>& operator=(Random_Iterable<Container>&& other) = delete;

        virtual Random_Iterable<Container>& operator++() = 0;
        virtual Random_Iterable<Container>& operator++(int) = 0; 
        virtual Random_Iterable<Container>& operator--() = 0;
        virtual Random_Iterable<Container>& operator--(int) = 0;

        virtual reference_type operator*() = 0;
        virtual pointer_type operator->() = 0;

        virtual bool operator==(const Random_Iterable<Container>& rhs) = 0;
        virtual bool operator!=(const Random_Iterable<Container>& rhs) = 0;

        virtual void operator[](size_t idx) = 0;
        virtual void front_inserter(const value_type& data) = 0;
        virtual void back_inserter(const value_type& data) = 0; 
        virtual void inserter(const value_type& data) = 0;

    private:
        pointer_type m_ptr{nullptr};
};