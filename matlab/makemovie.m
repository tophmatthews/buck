% set name here:
clear
prefix = '../problems/grouping/swelling/';

file = '1750';
N = 800;
s = N;
postfix = '.csv';

filename =  strcat(prefix,file,postfix);

empty = [];
a = parseCSV(filename,N,s, empty);
x = csvread(filename, 1, 0);
x=x(:,2:end-3);

writerObj = VideoWriter(strcat(prefix, file, '.avi'));
writerObj.FrameRate = 60;
open(writerObj);

figure
set(gca,'nextplot','replacechildren');
set(gca,'xscale', 'log', 'yscale', 'log', 'ylim', [2e-20 1e24], 'xlim', [0 2e3])
set(gcf,'Renderer','zbuffer')

mov(1:a.col)= struct('cdata',[],'colormap',[]);


for k=1:10:a.rows
  plot(a.atoms(1,:),x(k,:)*1e12,'.')
  timetext = strcat('time:', 32, num2str(a.time(k),'%10.5e\n'));
  burnuptext = strcat('burnup:', 32, num2str(a.burnup(k),'%10.5e\n'));
  swelltext = strcat('swelling:', 32, num2str(a.swell(k),'%10.5e\n'));
%   grtext = strcat('gas release:', 32, num2str(a.gr(k),'%10.5e\n'));
  title({timetext;swelltext;burnuptext})
  xlabel('Bubble radius [nm]')
  ylabel('Bubble concentration [bub/cm3]')
  frame = getframe(gcf);
  writeVideo(writerObj,frame);
end

close(writerObj);