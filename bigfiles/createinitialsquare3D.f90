program square
use iso_fortran_env, only : error_unit, output_unit
implicit none
real(8), parameter :: omega = 5.
real(8), parameter :: PI = acos(-1.)
real(8) :: x, y, z, u, v, w, Pressure, dx
integer :: i, j, k, m, n, nx
character(len=128) :: arg
character(len=128) :: filename
logical :: isNx
isNx = .false.
nx = -1
filename = ""
! Arguments
if ( iargc() == 0) then
  write(output_unit, '(A)') "Usage: square [OPTION]... FILENAME"
  write(output_unit, '(A)') "Generate a square patch of particles with nx the number of particles along one side of the square"
  write(output_unit, '(A)') ""
  write(output_unit, '(A)') "Options:"
  write(output_unit, '(A)') " -nx SIZE   number of particles along one side of the square"
  stop
else
  do i = 1, iargc()
    call getarg(i, arg)
    if (trim(arg) == "-nx") then
      isNx = .true.
    else
      if (isNx) then
        read(arg, *) nx
        isNx = .false.
      else
        if (arg(1:1) == "-") then
          write(error_unit, '(2A)') "ERROR: Unknown option", trim(arg)
          stop
        else
          filename = trim(arg)
        endif
      endif
    endif
  end do
endif
if (nx < 0) then
  write(error_unit, '(2A)') "ERROR: Omitted compulsory option -nx"
  stop
endif
if (trim(filename) == "") then
  write(error_unit, '(2A)') "ERROR: No output filename was specified"
  stop
endif
! Initialisation
dx = 1./nx
open(unit=10,file=trim(filename))
write(10,*),nx**2
do k=1,nx
  z = -0.5+1./(2.*nx) + (k-1)*1./nx
  do i=1,nx
    x = -0.5+1./(2*nx) + (i-1)*1./nx
    do j=1,nx
      y = -0.5+1./(2*nx) + (j-1)*1./nx
      ! Velocity
      u = omega*y
      v = -omega*x
      w = 0.
      Pressure = 0.
      do m=1,39,2 ! Odd numbers
        do n=1,39,2 ! Odd numbers
          Pressure = Pressure - 32. * omega**2 / (m*n*PI**2) / ((m*PI)**2+(n*PI)**2) * sin(m*PI*(x+0.5)) * sin(n*PI*(y+0.5))
        end do
      end do
      write(10,'(10ES15.7)') , x, y, z, u, v, w, Pressure*1000, dx, dx**3, 1.
    end do
  end do
end do
end program square
