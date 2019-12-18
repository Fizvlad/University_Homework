function table = ImplicitMethod(T_0, T_1, a, b, u_a, u_b, u_0, N, M, D)
%ImplicitMethod Solve using implicit method.
    syms x;
    syms t;
    
    h = (b - a) / (N - 1);
    tau = (T_1 - T_0) / (M - 1);
    table = zeros(N, M)+0.42;
    for i = 1:N
        x_i = a + (i - 1) * h;
        table(i, 1) = subs(u_0, x, x_i);
    end
    for k = 2:M
       t_i = T_0 + (i - 1) * tau;
       table(1, k) = subs(u_a, t, t_i);
       table(N, k) = subs(u_b, t, t_i);
    end
    
    % Used scheme
    %  o - # - o  k
    %      |
    %      o      k-1
    % i-1  i  i+1
    a = D / h^2;
    b = (2 * D / h^2 + 1 / tau);
    c = D / h^2;
    for k = 2:M
        alpha = zeros(N - 1);
        beta = zeros(N - 1);
        alpha(1) = 0;
        beta(1) = table(1, k);
        for i = 2:(N-1)
            alpha(i) = a / (b - c * alpha(i-1));
            beta(i) = (c * beta(i-1) - (-table(i, k-1) / tau)) / (b - c * alpha(i-1));
        end
        for i = (N - 1):-1:2
            table(i, k) = alpha(i) * table(i+1, k) + beta(i);
        end
    end
end

