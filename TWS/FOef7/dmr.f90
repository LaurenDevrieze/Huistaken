!   Tests of several square matrix-matrix products

program dmr
    use matrixop
    implicit none
    !--------------------------------------------------------------------------
    ! Abstract interfaces
    !
    ! NOTE: this simplifies the timings.
    !--------------------------------------------------------------------------
    abstract interface
        subroutine a_maal_b_interface(a, b, c)
            import dp
            real(kind=dp), dimension(:,:), intent(in)  :: a, b
            real(kind=dp), dimension(:,:), intent(out) :: c
        end subroutine a_maal_b_interface
        subroutine a_maal_b_blocks_interface(a,b,c,blocksize)
            import dp
            real(kind=dp), dimension(:,:), intent(in)  :: a, b
            real(kind=dp), dimension(:,:), intent(out) :: c
            integer, intent(in) :: blocksize
        end subroutine a_maal_b_blocks_interface
    end interface
    
    !--------------------------------------------------------------------------
    ! Main timing program
    !--------------------------------------------------------------------------
    integer :: k, N, blocksize, idx_i, idx_j
    real :: flops
    real :: dummy_i, dummy_j
    integer, dimension(:), allocatable :: seed
    real(kind=dp), dimension(:,:), allocatable :: a, b, c
    real(kind=dp), dimension(:,:), allocatable :: c_matmul

    ! Request the N and blocksize
    write(unit=*, fmt="(A)", advance="no") "Enter the value for N: "
    read *, N
    write(unit=*, fmt="(A)", advance="no") "Enter the blocksize of the sub-blocks: "
    read *, blocksize

    ! Make sure we use the same pseudo-random numbers each time by initializing
    ! the seed to a certain value.
    call random_seed(size=k)
    allocate(seed(k))
    seed = N
    call random_seed(put=seed)

    ! Calculate some random indices for the element c_ij that we are going to use
    ! to check if the matrix computation went ok.
    call random_number(dummy_i)
    call random_number(dummy_j)
    idx_i = floor(N*dummy_i) + 1
    idx_j = floor(N*dummy_j) + 1

    ! Allocate the matrices and one reference matrix
    allocate(a(N,N), b(N,N), c(N,N), c_matmul(N,N))
    call random_number(a)
    call random_number(b)
    call a_maal_b_matmul(a,b,c_matmul) ! Reference value

    ! Start the timings
    print *, ""
    write(unit=*, fmt="(A)") "TIMING RESULTS:"
    
    ! 1. Three nested loops
    call do_timing( "IJK", a_maal_b_ijk )
    call do_timing( "IKJ", a_maal_b_ikj )
    call do_timing( "JIK", a_maal_b_jik )
    call do_timing( "JKI", a_maal_b_jki )
    call do_timing( "KIJ", a_maal_b_kij )
    call do_timing( "KJI", a_maal_b_kji )
    
    ! 2. Two nested loops with vector operations
    call do_timing( "IKJ, J VECT", a_maal_b_ikj_vect )
    call do_timing( "JKI, I VECT", a_maal_b_jki_vect )
    call do_timing( "KIJ, J VECT", a_maal_b_kij_vect )
    call do_timing( "KJI, I VECT", a_maal_b_kji_vect )
    
    ! 3. Two nested loops with dot_product
    call do_timing( "IJ DOT_PRODUCT", a_maal_b_ij_dot_product )
    call do_timing( "JI DOT_PRODUCT", a_maal_b_ji_dot_product )
    
    ! 4. Two nested loops with dot_product and explicit transpose of matrix A
    call do_timing( "IJ TP DOT_PRODUCT", a_maal_b_transp_ij_dot_product )
    call do_timing( "JI TP DOT_PRODUCT", a_maal_b_transp_ji_dot_product )
    
    ! 5. Using BLAS
    call do_timing( "BLAS DGEMM", a_maal_b_blas )
    
    ! 6. In blocks
    call do_timing( "IN BLOCKS", method_blocks=a_maal_b_blocks )
    
    ! 7. Intrinsic matmul function
    call do_timing( "MATMUL", a_maal_b_matmul )
    
    ! Clean up
    deallocate(a, b, c, c_matmul)

contains

    subroutine do_timing( name, method, method_blocks )
        character(len=*), intent(in) :: name
        procedure(a_maal_b_interface), optional :: method
        procedure(a_maal_b_blocks_interface), optional :: method_blocks
        real(kind=dp) :: mynorm
        real :: t1, t2
        ! Do the timing
        if( present(method) ) then
            call cpu_time(t1)
            call method( a, b, c )
            call cpu_time(t2)
        else
            call cpu_time(t1)
            call method_blocks( a, b, c, blocksize)
            call cpu_time(t2)
        end if
        ! Compute the Frobenius norm of the error and divide by the Frobenius norm
        ! of the exact matrixproduct to get some kind of relative error.
        mynorm = sqrt(sum((c_matmul-c)**2))/sqrt(sum(c_matmul**2))

	print "(A18, F7.2, A, ES9.2, A, ES9.2)", name // ": ", t2-t1, " sec, test = ", c(idx_i, idx_j), ", relative error = ", mynorm

    end subroutine do_timing

end program dmr
