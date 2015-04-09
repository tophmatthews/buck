% set name here:
clear
prefix = '../problems/grouping/constantb/';

file = '1e9x5-2000';
N = 1e9;
s = 5;
postfix = '.csv';

filename =  strcat(prefix,file,postfix);

a = parseCSV(filename,N,s);
x = csvread(filename, 1, 0);
x=x(:,2:end-3);

writerObj = VideoWriter(strcat(file, '.avi'));
writerObj.FrameRate = 60;
open(writerObj);

set(gca,'nextplot','replacechildren');
set(gca,'xscale', 'log', 'yscale', 'log', 'ylim', [2e-30 1e10], 'xlim', [0 a.atoms(end)*2])
set(gcf,'Renderer','zbuffer')

mov(1:a.col)= struct('cdata',[],'colormap',[]);


for k=1:1:a.rows
  plot(a.atoms(1,:),x(k,:),'.')
  timetext = strcat('time:', 32, num2str(a.time(k),'%10.5e\n'));
  swelltext = strcat('swelling:', 32, num2str(a.swell(k),'%10.5e\n'));
  grtext = strcat('gas release:', 32, num2str(a.gr(k),'%10.5e\n'));
  title({timetext;swelltext;grtext})
  xlabel('Bubble size [atoms]')
  ylabel('Bubble concentration [bub/um]')
  frame = getframe(gcf);
  writeVideo(writerObj,frame);
end

close(writerObj);