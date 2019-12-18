function table = ExplicitMethod(T_0, T_1, a, b, u_a, u_b, u_0, N, M, D)
%ExplicitMethod Solve using explicit method.
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
    %      #      k
    %      |
    %  o - o - o  k-1
    % i-1  i  i+1
    
    q = tau / h^2;
    if q >= 0.5
        error("tau / h^2 = " + num2str(q) + ". Should be less than 0.5")
    end
    for k = 2:M
        for i = 2:(N-1)
            table(i, k) = D * q * (table(i+1, k-1) + table(i-1, k-1)) + (1 - 2*D*q) * table(i, k-1);
        end
    end
end

