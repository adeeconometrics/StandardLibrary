#pragma once

template <typename Container> class Bidirectional_Iterable{
    public:
        typedef Container value_type;
        typedef value_type* pointer_type;
        typedef value_type& reference_type;

    public:
        Bidirectional_Iterable() = default;
        Bidirectional_Iterable(pointer_type ptr): m_ptr(ptr){}
        Bidirectional_Iterable(const Container& other) = delete;
        Bidirectional_Iterable(Container&& other) = delete;

        virtual ~Bidirectional_Iterable() = default;

        Bidirectional_Iterable<Container>& operator=(const Bidirectional_Iterable<Container>& other) = delete;
        Bidirectional_Iterable<Container>& operator=(Bidirectional_Iterable<Container>&& other) = delete;

        virtual Bidirectional_Iterable<Container>& operator++() = 0;
        virtual Bidirectional_Iterable<Container>& operator++(int) = 0; 
        virtual Bidirectional_Iterable<Container>& operator--() = 0;
        virtual Bidirectional_Iterable<Container>& operator--(int) = 0;

        virtual reference_type operator*() = 0;
        virtual pointer_type operator->() = 0;

        virtual bool operator==(const Bidirectional_Iterable<Container>& rhs) = 0;
        virtual bool operator!=(const Bidirectional_Iterable<Container>& rhs) = 0;

        virtual void front_inserter(const value_type& data) = 0;
        virtual void back_inserter(const value_type& data) = 0; 

    private:
        pointer_type m_ptr{nullptr};
};