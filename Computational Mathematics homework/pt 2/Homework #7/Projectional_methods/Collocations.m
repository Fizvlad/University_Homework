function re = Collocations(a, b, n)
%COLLOCATIONS Solute problem with callocations method
    % Determining points
    syms x;
    ch_f = chebyshevT(n, x);
    ch_coeffs = vpa(coeffs(ch_f, 'All'));
    ch_roots = sort(roots(ch_coeffs));
    ch_roots = arrayfun(@(x)(a + (x + 1)*(b - a)/2), ch_roots);
    
    [basis, dbasis, ddbasis] = Basis(n);
    A = zeros(n, n);    
    rhs = zeros(n, 1);    
    for i = 1:n
        for j = 1:n
            func = (-P * ddbasis(j) + Q * dbasis(j) + R * basis(j));
            A(i,j) = subs(func, x, ch_roots(i));
        end
        func = F;
        rhs(i) = subs(func, x, ch_roots(i));
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

