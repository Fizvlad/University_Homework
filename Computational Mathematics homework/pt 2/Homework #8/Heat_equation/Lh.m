function lh = Lh(u_l, u, u_r, h)
%Lh L_h for current problem.
    lh = (u_r - 2 * u + u_l) / (h ^ 2);
end

