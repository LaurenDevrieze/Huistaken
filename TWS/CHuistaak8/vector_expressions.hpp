#ifndef tws_vector_expressions_hpp
#define tws_vector_expressions_hpp
namespace tws{
       
    template <typename V1, typename V2>
    class vector_sum {
        public:
        typedef typename V1::size_type size_type ;
        typedef decltype( typename V1::value_type() + typename V2::value_type() ) value_type ;

        public:
        vector_sum( V1 const& v1, V2 const& v2 )
        : v1_( v1 )
        , v2_( v2 )
        {
            assert( v1.size()==v2.size() ) ;
        }

        size_type size() const {
            return v1_.size() ;
        }

        value_type operator()( size_type i ) const {
            assert( i>=0 ) ;
            assert( i<size() ) ;
            return v1_(i) + v2_(i) ;
        }

        private:
        V1 const& v1_ ;
        V2 const& v2_ ;
    } ; //vector_sum

    template <typename V1, typename V2>
    vector_sum<V1,V2> operator+(V1 const& v1, V2 const& v2 ) {
        return vector_sum<V1,V2>(v1,v2) ;
    } //operator+


    template <typename V1, typename V2>
    class vector_minus {
        public:
        typedef typename V1::size_type size_type ;
        typedef decltype( typename V1::value_type() + typename V2::value_type() ) value_type ;

        public:
        vector_minus( V1 const& v1, V2 const& v2 )
        : v1_( v1 )
        , v2_( v2 )
        {
            assert( v1.size()==v2.size() ) ;
        }

        size_type size() const {
            return v1_.size() ;
        }
    
        value_type operator()( size_type i ) const {
            assert( i>=0 ) ;
            assert( i<size() ) ;
            return v1_(i) - v2_(i) ;
        }

        private:
        V1 const& v1_ ;
        V2 const& v2_ ;
    } ; //vector_minus

    template <typename V1, typename V2>
    vector_minus<V1,V2> operator-(V1 const& v1, V2 const& v2 ) {
        return vector_minus<V1,V2>(v1,v2) ;
    }//operator-

    template <typename S1, typename V2>
    class scalar_vector_multiply {
        public:
        typedef typename V2::size_type size_type ;
        typedef decltype( S1() + typename V2::value_type() ) value_type ;

        public:
        scalar_vector_multiply( S1 const& s1, V2 const& v2 )
        : s1_( s1 )
        , v2_( v2 )
        {
        }

        size_type size() const {
            return v2_.size() ;
        }

        value_type operator()( size_type i ) const {
            assert( i>=0 ) ;
            assert( i<size() ) ;
            return s1_*v2_(i) ;
        }

        private:
        S1 const& s1_ ;
        V2 const& v2_ ;
    } ; //scalar_vector_multiply

    template <typename S1, typename V2>
    scalar_vector_multiply<S1,V2> operator*(S1 const& s1, V2 const& v2 ) {
        return scalar_vector_multiply<S1,V2>(s1,v2) ;
    }//operator*
	

} // namespace tws

#endif
