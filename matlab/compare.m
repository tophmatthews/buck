prefix = '../problems/grouping/';

% files = {'hard/x5', 'avg/x5', 'hard/x10', 'avg/x10'};
files = {'fixed/x5', 'fixed/100', 'fixed/x5-avg'};
N = [100, 100, 100];
s = [5, 100, 5];
postfix = '_csv.csv';

filenames = {};
for i=1:1:size(files,2)
    filenames(end+1) = strcat(prefix,files(i),postfix);
end

a = {};
for i=1:1:size(filenames,2)
    a{end+1} = parseCSV( char(filenames(i)) , N(i), s(i));
end


figure('units','normalized','outerposition',[0 0 1 1])

% xscale = 'linear'
% yscale = 'linear'

num = size(files,2);

subplot(2,3,1)
hold on
for i=1:1:num
    plot(a{i}.time, a{i}.swell)
end
hold off
title('Swelling')
legend(files, 'location', 'northwest')
xlabel('Time [s]')
ylabel('Fractional Swelling')
set(gca,'xscale', 'log', 'yscale', 'log')
%xlim([0 1e4])

subplot(2,3,2)
hold on
for i=1:1:num
    plot(a{i}.time, a{i}.gr)
end
hold off
title('Gas release')
%xlim([0 1e4])
legend(files, 'location', 'southeast')
xlabel('Time [s]')
ylabel('Gas release [atoms]')
set(gca,'xscale', 'linear', 'yscale', 'linear')

subplot(2,3,3)
hold on
for i=1:1:num
    plot(a{i}.time, a{i}.small)
end
hold off
title('Gas atom conc over time')
%xlim([0 1e6])
legend(files, 'location', 'southeast')
xlabel('Time [s]')
ylabel('c1 [atoms/um3]')
set(gca,'xscale', 'log', 'yscale', 'log')

subplot(2,3,4)
hold on
for i=1:1:num
    plot(a{i}.atoms, a{i}.last)
end
hold off
legend(files,'location','northeast')
xlabel('Radius [um]')
ylabel('Concentration [bub/um3]')
title('Bubbles at last time step')
set(gca,'xscale', 'log', 'yscale', 'log')

subplot(2,3,5)
hold on
for i=1:1:num
    plot(a{i}.time, a{i}.big)
end
hold off
legend(files,'location','northwest')
xlabel('Time [s]')
ylabel('Concentration [bub/um3]')
title('Biggest bubble over time')
set(gca,'xscale', 'log', 'yscale', 'log')

subplot(2,3,6)
hold on
for i=1:1:num
    plot(a{i}.time, a{i}.dt)
end
hold off
legend(files,'location','northwest')
xlabel('Time [s]')
ylabel('dt [s]')
title('Timestep size over time')
set(gca,'xscale', 'log', 'yscale', 'log')

% 
% 
% for j=1:1:6
%     subplot(2,3,j)
%     hold on
%     for i=1:1:num
%         at = floor(a{i}.rows * j / 6);
%         plot(a{i}.atoms, a{i}.all{at})
%     end
%     hold off
%     legend(files,'location','northeast')
%     xlabel('Radius [um]')
%     ylabel('Concentration [bub/um3]')
%     title(strcat('Bubbles at last time step:', 32, num2str(a{i}.time(at))))
%     set(gca,'xscale', 'log', 'yscale', 'log')
% end

