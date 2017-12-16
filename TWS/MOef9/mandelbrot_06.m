% MANDELBROT_06
function R_tilde=mandelbrot_06(center,radius,steps,maxiter)

Z = zeros(steps);
C = zeros(steps);
R_tilde = zeros(steps);

for n=1:steps
    for m=1:steps
        C(m,n) = real(center)-radius+2*(n-1)*radius/(steps-1) ...
           + 1i*(imag(center)-radius+2*(m-1)*radius/(steps-1));
        Z(m,n) = C(m,n);
        R_tilde(m,n) = maxiter;
    end
end
for r=1:maxiter
    [m,n] = find(abs(Z) <= 2);
    for j = 1:size(m,1)
        id = (n(j)-1)*size(Z,1) + m(j);
        Z(id) = Z(id)*Z(id) + C(id);
        R_tilde(id) = r;
    end
end
