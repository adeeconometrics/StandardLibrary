#pragma once

template <typename Container> class Forward_Iterable{
    public: 
        typedef Container value_type;
        typedef value_type *pointer_type;

    public:
        Forward_Iterable() = default;
        Forward_Iterable(pointer_type ptr): m_ptr(ptr){}
        Forward_Iterable(const Container& other) =  delete;
        Forward_Iterable(Container&& other) = delete;
        
        virtual ~Forward_Iterable() = default;

        Forward_Iterable<Container>& operator=(const Forward_Iterable<Container>& other) = delete;
        Forward_Iterable<Container>& operator=(Forward_Iterable<Container>&& other) = delete;
        Forward_Iterable<Container>& operator++() = 0;
        Forward_Iterable<Container>& operator++(int) = 0;

        bool operator==(const Forward_Iterable<Container>& other) = 0;
        bool operator!=(const Forward_Iterable<Container>& other) = 0;

        virtual void next() = 0;
        virtual bool has_next() = 0;

    private:
        pointer_type m_ptr{nullptr};
};