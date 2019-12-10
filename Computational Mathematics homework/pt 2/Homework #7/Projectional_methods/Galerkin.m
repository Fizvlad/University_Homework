function re = Galerkin(a, b, n)
%GALERKIN solute problem with Galerkin method.
    syms x;
    [basis, dbasis, ddbasis] = Basis(n);
    A = zeros(n, n);    
    rhs = zeros(n, 1);    
    for i = 1:n
        for j = 1:n
            func = (-P * ddbasis(j) + Q * dbasis(j) + R * basis(j)) * basis(i);
            A(i,j) = int(func, a, b);
        end
        func = F * basis(i);
        rhs(i) = int(func, a, b);
    end
    c = A \ rhs;
    re = 0;
    for i = 1:n
        re = re + c(i) * basis(i);
    end
    re = collect(re);
end

function re = Jacobi(k, n)
%jacobi Jacobi polynomial
    syms x;
    ja = sym(zeros(n+1));
    ja(0 +1) = 1;
    ja(1 +1) = (k + 1) * x;
    for i = 0:(n-3)
        ja(i+2 +1) = (( (i + k + 2) * (2*i + 2*k + 3) * x * ja(i + 2) ) - ...
                      ( (i + k + 2) * (i + k + 1) * ja(i + 1) )) / ...
                        ((i + 2*k + 2) * (i + 2));
    end
    re = ja(n);
    %syms x;
    %re = (-1)^n * (1-x^2)^(-k) * diff((1-x^2)^(n+k), x, 2) / (2^n * factorial(n));
end

function [basis, dbasis, ddbasis] = Basis(n)
%basis returnes basis and its differentials for this problem
    syms x;
    basis = sym(zeros(n));
    dbasis = sym(zeros(n));
    ddbasis = sym(zeros(n));
    for i=1:n
        basis(i) = (1 - x^2) * Jacobi(1, i);
        dbasis(i) = -2 * (i) * Jacobi(0, i + 1);
        ddbasis(i) = -1 * (i + 1) * i * Jacobi(1, i);
    end
end