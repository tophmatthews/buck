 
% figure
% burnupsa = [];
% for i=1:1:length(filesa)
%     burnupsa(end+1) = a{i}.burnup(end);
% end
% burnupsb = [];
% for i=1:1:length(filesb)
%     burnupsb(end+1) = b{i}.burnup(end);
% end
% lower = 1;
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




% figure('units','normalized','outerposition',[0 0 1 1])
% 
% 
% num = size(filesa,2);
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



