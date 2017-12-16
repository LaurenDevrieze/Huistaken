% MANDELBROT_00 Non-optimised Mandelbrot function. On purpose, this code contains a bug and some superfluous code.
function R_tilde=mandelbrot_00(center,radius,steps,maxiter)

Z = [];
C = [];
R_tilde = [];

for r=0:maxiter
    for m=1:steps
        for n=1:steps
            if r == 0
                C(m,n) = real(center)-radius+2*(n-1)*radius/(steps-1) ...
                    + i*(imag(center)-radius+2*(m-1)*radius/(steps-1));
                Z(m,n) = C(m,n);
                R_tilde(m,n) = maxiter;
            else
               if R_tilde(m,n) <= maxiter
                    Z(m,n) = Z(m,n)*Z(m,n) + C(m,n);
                    if abs(Z(m,n)) > 2 && (R_tilde(m,n))==maxiter
                        R_tilde(m,n) = r;
                    end
                end
                if R_tilde(m,n) == 0
                    R_tilde(m,n) = r+1;
                end
            end
        end
    end
end
