function [ a ] = calcAtoms( N, s )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
a = [];
for j=1:1:s
    a(end+1) = j;
end

for j=s+1:1:N
    x = ( a(end) + 1 ) / s + a(end);
    
    if x<=N
        a(end+1) = x;
    else
      a(end+1) = N;
      break;
    end
end

