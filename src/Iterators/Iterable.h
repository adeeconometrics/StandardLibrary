#pragma once

template <typename Container> class Iterable{
    public: 
        typedef Container value_type;

    public:
        Iterable() = default;
        Iterable(const Container& other) =  delete; // stl iterators are copy-constructible
        Iterable(Container&& other) = delete;
        
        virtual ~Iterable() = default;

        Iterable<Container>& operator=(const Iterable<Container>& other) = delete;
        Iterable<Container>& operator=(Iterable<Container>&& other) = delete;
        Iterable<Container>& operator++() = 0;
        Iterable<Container>& operator++(int) = 0;

        bool operator==(const Iterable<Container>& other) = 0;
        bool operator!=(const Iterable<Container>& other) = 0;
        
        virtual void next() = 0;
        virtual bool has_next() = 0;

    private: 
        size_t m_size{0};
};
