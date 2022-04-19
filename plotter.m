%% Read in location data
pre = importdata("pre-node-locations.data");
post = importdata("post-node-locations.data");

%% Location Plots
close all

figure
plot(pre(:,1), pre(:,2),'r.','MarkerSize',10)
title("Before Simulation")
xlabel("X")
ylabel("Y")
saveas(gcf,'before.png')


%% adjust the values to avoid overlap
X = post(:,1);
Xadj = zeros(size(X));
Y = post(:,2);
Yadj = zeros(size(Y));
[~, ~, Idx] = unique([X Y], 'rows');  %find unique (x,y) coordinates
for j = 1:length(Idx)
    Loc = (Idx == j);
    Copies = sum(Loc);
    if Copies > 1
        Xadj(Loc) = 2*rand(Copies, 1) - 1; %add values from -1 to 1
        Yadj(Loc) = 2*rand(Copies, 1) - 1; %add values from -1 to 1
    end
end

figure
hold on
plot(X+Xadj, Y+Yadj,'r.','MarkerSize',10) %Plot X and Y, adjusting values if they overlap.

% draw circles
circle(10,40,1.2);
circle(5,7,1.2);
circle(20,40,1.2);
circle(15,25,1.2);
circle(35,20,1.2);

title("After Simulation")
xlabel("X")
ylabel("Y")
xlim([0 45])
ylim([0 45])

hold off

% function to draw a circle
function h = circle(x,y,r)
hold on
th = 0:pi/50:2*pi;
xunit = r * cos(th) + x;
yunit = r * sin(th) + y;
h = plot(xunit, yunit,'LineWidth',2);
hold off
end
