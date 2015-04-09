function [ a ] = parseCSV( filename, N, s )
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
a = {};

x = csvread(filename, 1, 0);
a.rows = size(x,1);
a.col = size(x,2);
a.time = x(:,1);
a.burnup = a.time * 10 / 30862;
a.gr = x(:,a.col-1);
a.swell = x(:,a.col-1);
a.dt = x(:,a.col-2);
a.big = x(:,a.col-3);
a.small = x(:,2);
a.last = x(end,2:end-3);
a.atoms = calcAtoms(N,s);

a.all = {};

for i=1:1:a.rows
    a.all{end+1} = x(i,2:end-3);
end
    

end

