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
    
    disp('Solving system:');
    disp([A rhs]);
    disp('Condition number of system:');
    disp(cond(A));
    c = A \ rhs;
    disp('Decomposition coefficients:');
    disp(c);
    re = 0;
    for i = 1:n
        re = re + c(i) * basis(i);
    end
    re = collect(re);
end