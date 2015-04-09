clear
prefix = '../problems/grouping/limit/';

% files = {'hard/x5', 'avg/x5', 'hard/x10', 'avg/x10'};
% filesa = {'1e9x5-1000', '1e9x5-1200', '1e9x5-1400', '1e9x5-1600', '1e9x5-1800', '1e9x5-2000'};
% filesb = {'1e9x5-1000-x0.01', '1e9x5-1200-x0.01', '1e9x5-1400-x0.01', '1e9x5-1600-x0.01', '1e9x5-1800-x0.01', '1e9x5-2000-x0.01'};
% filesb = {'1e9x5-1000-x0.1', '1e9x5-1200-x0.1', '1e9x5-1400-x0.1', '1e9x5-1600-x0.1', '1e9x5-1800-x0.1', '1e9x5-2000-x0.1'};
% filesb = {'1e9x5-1000-x10', '1e9x5-1200-x10', '1e9x5-1400-x10', '1e9x5-1600-x10', '1e9x5-1800-x10', '1e9x5-2000-x10'};
% filesb = {'1e9x5-1000-x100', '1e9x5-1200-x100', '1e9x5-1400-x100', '1e9x5-1600-x100', '1e9x5-1800-x100', '1e9x5-2000-x100'};
%filesb = {'1e9x10-1000', '1e9x10-1100', '1e9x10-1200', '1e9x10-1300', '1e9x10-1400', '1e9x10-1500', '1e9x10-1600', '1e9x10-1700', '1e9x10-1800', '1e9x10-1900', '1e9x10-2000'};
% filesa = {'1e9x5-1000', '1e9x5-1050', '1e9x5-1100', '1e9x5-1150', '1e9x5-1200'};
% filesa = {'1e9x5-1000-f0.1', '1e9x5-1050-f0.1', '1e9x5-1100-f0.1', '1e9x5-1150-f0.1', '1e9x5-1200-f0.1'};
filesa = {'1e9x5-1000-f10', '1e9x5-1050-f10', '1e9x5-1100-f10', '1e9x5-1150-f10', '1e9x5-1200-f10'};
% filesb = {'1e9x10-1000', '1e9x10-1050', '1e9x10-1100', '1e9x10-1150', '1e9x10-1200'};
% filesb = {'1e9x10-1000-f0.1', '1e9x10-1050-f0.1', '1e9x10-1100-f0.1', '1e9x10-1150-f0.1', '1e9x10-1200-f0.1'};
filesb = {'1e9x10-1000-f10', '1e9x10-1050-f10', '1e9x10-1100-f10', '1e9x10-1150-f10', '1e9x10-1200-f10'};
% filesa = {'knock/1e9x5-1000', 'knock/1e9x5-1200', 'knock/1e9x5-1400', 'knock/1e9x5-1600', 'knock/1e9x5-1800', 'knock/1e9x5-2000'};
% filesb = {'constantb/1e9x5-1000', 'constantb/1e9x5-1200', 'constantb/1e9x5-1400', 'constantb/1e9x5-1600', 'constantb/1e9x5-1800', 'constantb/1e9x5-2000'};
N = 1e9;
sa = [5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5];
sb = sa;
% sb = [10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10];
tempa = [1000, 1200, 1400, 1600, 1800, 2000];
tempb = tempa;
postfix = '.csv';

allfiles = {};
for i=1:1:length(filesa)
    allfiles(end+1) = filesa(i);
end
for i=1:1:length(filesb)
    allfiles(end+1) = filesb(i);
end

filenamesa = {};
for i=1:1:size(filesa,2)
    filenamesa(end+1) = strcat(prefix,filesa(i),postfix);
end

a = {};
for i=1:1:length(filenamesa)
    a{end+1} = parseCSV( char(filenamesa(i)) , N, sa(i));
end

filenamesb = {};
for i=1:1:size(filesb,2)
    filenamesb(end+1) = strcat(prefix,filesb(i),postfix);
end


b = {};
for i=1:1:size(filenamesb,2)
    b{end+1} = parseCSV( char(filenamesb(i)) , N, sb(i));
end

% figure
% burnupsa = [];
% for i=1:1:length(filesa)
%     burnupsa(end+1) = a{i}.burnup(end);
% end
% burnupsb = [];
% for i=1:1:length(filesb)
%     burnupsb(end+1) = b{i}.burnup(end);
% end
% lower = 3;
% plot(burnupsa(lower:end), tempa(lower:length(burnupsa)),'x', burnupsb(lower:end), tempb(lower:length(burnupsa)),'o')
% 
% refx = [2.788, 4.046, 4.757, 6.829, 8.909, 10.983, 1.3,1.169, 3.07, 4.372, 6.228, 7.198, 11.714, 0.846, 1.24, 2.965, 4.017, 5.998, 6.125];
% refy = [1302.5, 1223.7, 1195.9, 1141, 1125.6, 1092.6, 1323, 1294, 1222.7, 1212.2, 1170.4, 1132.4, 1092, 1364.9, 1326.4, 1241.2, 1204.3, 1170, 1155.3];
% 
% 
% figure
% plot(burnupsa,tempa);
% hold on
% plot(refx*1500,refy,'x')
% hold off

cc=jet(12);
figure
thisnum = length(filesa);
title('Swelling')
hold on
for i=1:1:thisnum
    plot(a{i}.time, a{i}.swell,'color',cc(i,:))
end
for i=1:1:thisnum
    plot(b{i}.time, b{i}.swell,'--x','color',cc(i,:))
end
hold off
allfiles = {};
for i=1:1:thisnum
    allfiles(end+1) = filesa(i);
end
for i=1:1:thisnum
    allfiles(end+1) = filesb(i);
end
legend(allfiles, 'location', 'northwest')
xlabel('Time [s]')
ylabel('Fractional Swelling')
xlim([1e5 1e10])
set(gca,'xscale', 'log', 'yscale', 'log')


% figure('units','normalized','outerposition',[0 0 1 1])
% 
% 
% num = size(files,2);
% 
% subplot(2,3,1)
% title('Swelling')
% hold on
% for i=1:1:num
%     plot(a{i}.time, a{i}.swell)
% end
% hold off
% legend(files, 'location', 'northwest')
% xlabel('Time [s]')
% ylabel('Fractional Swelling')
% set(gca,'xscale', 'log', 'yscale', 'log')
% % xlim([0 1e4])
% 
% subplot(2,3,2)
% title('Gas release')
% hold on
% for i=1:1:num
%     plot(a{i}.time, a{i}.gr)
% end
% hold off
% %xlim([0 1e4])
% legend(files, 'location', 'southeast')
% xlabel('Time [s]')
% ylabel('Gas release [atoms]')
% set(gca,'xscale', 'linear', 'yscale', 'linear')
% 
% subplot(2,3,3)
% title('Gas atom conc over time')
% hold on
% for i=1:1:num
%     plot(a{i}.time, a{i}.small)
% end
% hold off
% %xlim([0 1e6])
% legend(files, 'location', 'southeast')
% xlabel('Time [s]')
% ylabel('c1 [atoms/um3]')
% set(gca,'xscale', 'log', 'yscale', 'linear')
% 
% 
% subplot(2,3,4)
% title('Bubbles at last time step')
% hold on
% for i=1:1:num
%     plot(a{i}.atoms, a{i}.last)
% end
% hold off
% legend(files,'location','northeast')
% xlabel('Atom')
% ylabel('Concentration [bub/um3]')
% set(gca,'xscale', 'log', 'yscale', 'log')
% % xlim([0 100])
% 
% subplot(2,3,5)
% title('largest atom bubble over time')
% hold on
% for i=1:1:num
%     plot(a{i}.time, a{i}.big)
% end
% hold off
% legend(files,'location','northwest')
% xlabel('Time [s]')
% ylabel('Concentration [bub/um3]')
% set(gca,'xscale', 'linear', 'yscale', 'linear')
% 
% subplot(2,3,6)
% title('Timestep size over time')
% hold on
% for i=1:1:num
%     plot(a{i}.time, a{i}.dt)
% end
% hold off
% legend(files,'location','northwest')
% xlabel('Time [s]')
% ylabel('dt [s]')
% set(gca,'xscale', 'log', 'yscale', 'log')



% for j=1:1:6
%     subplot(2,3,j)
%     hold on
%     for i=1:1:size(files,2)
%         at = floor(a{i}.rows * j / 6);
%         if i==1
%             plot(a{i}.atoms, a{i}.all{at})
%         else
%             plot(a{i}.atoms, a{i}.all{at},'o')
%         end
%     end
%     hold off
%     legend(files,'location','northeast')
%     xlabel('Radius [um]')
%     ylabel('Concentration [bub/um3]')
%     title(strcat('Bubbles at last time step:', 32, num2str(a{i}.time(at))))
%     set(gca,'xscale', 'log', 'yscale', 'log')
% %     xlim([80 100])
% end



