
clf; close all; clear all;
format long;

seconds = zeros(7, 24 * 3600);
scalar = 1;
div = 512;
add = 512;
figure(1)
title("Different seeds for monday");
x = [0, 2, 4, 6, 8, 10, 12, 14, 16 ,18, 20, 22];
samples = get_samples("mon", scalar);
plot(x, samples, "b-"); hold on; 
samples = get_samples("mon", scalar);
plot(x, samples, "r-"); hold on;
samples = get_samples("mon", scalar);
plot(x, samples, "k-"); hold on; 
samples = get_samples("mon", scalar);
plot(x, samples, "m-"); hold on;
samples = get_samples("mon", scalar);
plot(x, samples, "g-"); hold on; 
samples = get_samples("mon", scalar);
plot(x, samples, "y-"); hold on;
samples = get_samples("mon", scalar);
plot(x, samples, "o-");

figure(2)

x = [0, 2, 4, 6, 8, 10, 12, 14, 16 ,18, 20, 22];
samples = get_samples("mon", scalar);
plot(x, samples, "b-"); hold on; 
samples = get_samples("tue", scalar);
plot(x, samples, "r-"); hold on;
samples = get_samples("wed", scalar);
plot(x, samples, "k-"); hold on; 
samples = get_samples("thu", scalar);
plot(x, samples, "m-"); hold on;
samples = get_samples("fri", scalar);
plot(x, samples, "g-"); hold on; 
samples = get_samples("sat", scalar);
plot(x, samples, "p-"); hold on;
samples = get_samples("sun", scalar);
plot(x, samples, "o-"); grid on;
title("Referencyjne pr�bki rozak�adu ruchu w tygodniu");
legend('poniedzia�ek','wtorek','�roda','czwartek','pi�tek','sobota','niedziela');
xlabel('godzina w ci�gu dnia') 
ylabel('wzgl�dne nat�enie ruchu na drodze') 
samples = get_samples("mon", scalar);
m = linspace(0, 24, 86400);
u = [0 , 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24];
samples = [samples, samples(1)];
samples = (samples + add);
p = polyfit(u, samples, 6);
fileID = fopen('polynomial.txt','w');
fprintf(fileID, 'p0 = ');
fprintf(fileID,'%.20f, ',p);
fprintf(fileID, '\n');
out = polyval(p,m);
disp("mon");
out2 = p(1)*m.^6 + p(2)*m.^5 + p(3)*m.^4 + p(4)*m.^3 + p(5)*m.^2 + p(6)*m + p(7);

out2 = 2 * out2 / div;
out3 = out2.*rand(1,86400);
%out3 = out3 * 0.98;
out4 = floor(out3);
out4(1:25)
start = 40000;
stop = 40050;
out6 = out4(start:stop);
out7 = out2(start:stop);
figure(20)
stem(m(start:stop), out6, 'm-'); hold on; grid on;
r = plot(m(start:stop), out7,'r-'); hold on;
title("Wynik interpolacji i ostateczny generowany ruch na sekund�");
legend('wynik interpolacji','liczba samochod�w');
xlabel('godzina') 
ylabel('liczba samochod�w na sekund�') 
figure(3)
plot(m, out/div, "r-"); hold on;
plot(m, out2, "b-"); hold on;
title('Rozk�ad ruchu dla poszczeg�lnych sekund');
xlabel('godzina'); ylabel('liczba aut na godzin�');
out5 = zeros(1,24);
for i=0:23
    for j=0:3599
        out5(i + 1) = out5(i + 1) + out4(i * 3600 + j + 1);
    end
end
w = linspace(1,24,24);
figure(10)
%stem(w, out5, "go"); hold on;
bar(w, out5, 'r'); hold on;
title('�redni wygenerowany ruch');
xlabel('godzina'); ylabel('liczba aut na godzin�');

samples = get_samples("tue", scalar);
m = linspace(0, 24, 86400);
u = [0 , 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24];
samples = [samples, samples(1)];
samples = (samples + add);
p = polyfit(u, samples, 6);

fprintf(fileID, 'p1 = ');
fprintf(fileID,'%.20f, ',p);
fprintf(fileID, '\n');
out = polyval(p,m);
disp("tue");
out2 = p(1)*m.^6 + p(2)*m.^5 + p(3)*m.^4 + p(4)*m.^3 + p(5)*m.^2 + p(6)*m + p(7);
out2 = out2 / div;
out3 = out2.*rand(1,86400);
out4 = floor(out3);
figure(4)
plot(m, out/div, "r-"); hold on;
plot(m, out2, "bo"); hold on;
out5 = zeros(1,24);
for i=0:23
    for j=0:3599
        out5(i + 1) = out5(i + 1) + out4(i * 3600 + j + 1);
    end
end
w = 1:24;
figure(11)
plot(w, out5, "go"); hold on;

% samples = get_samples("tue", scalar);
% m = linspace(0, 86400, 86400);
% u = [0 , 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24];
% u = u.*3600; 
% samples = [samples, samples(1)];
% samples = (samples + add);
% p = polyfit(u, samples, 6);
% out = polyval(p,m);
% disp("tue");
% p
% out2 = p(1)*m.^6 + p(2)*m.^5 + p(3)*m.^4 + p(4)*m.^3 + p(5)*m.^2 + p(6)*m + p(7);
% out2 = out2 / div;
% out3 = out2.*rand(1,86400);
% figure(4)
% plot(u, samples/div); hold on;
% plot(m, out/div, "r-"); hold on;
% plot(m, out2, "bo"); hold on;
% 
samples = get_samples("wed", scalar);
m = linspace(0, 24, 86400);
u = [0 , 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24];
samples = [samples, samples(1)];
samples = (samples + add);
p = polyfit(u, samples, 6);

fprintf(fileID, 'p2 = ');
fprintf(fileID,'%.20f, ',p);
fprintf(fileID, '\n');
out = polyval(p,m);
disp("wed");
out2 = p(1)*m.^6 + p(2)*m.^5 + p(3)*m.^4 + p(4)*m.^3 + p(5)*m.^2 + p(6)*m + p(7);
out2 = out2 / div;
out3 = out2.*rand(1,86400);
out4 = floor(out3);
figure(5)
plot(m, out/div, "r-"); hold on;
plot(m, out2, "bo"); hold on;
out5 = zeros(1,24);
for i=0:23
    for j=0:3599
        out5(i + 1) = out5(i + 1) + out4(i * 3600 + j + 1);
    end
end
w = 1:24;
figure(12)
plot(w, out5, "go"); hold on;
% samples = get_samples("wed", scalar);
% m = linspace(0, 86400, 86400);
% u = [0 , 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24];
% u = u.*3600; 
% samples = [samples, samples(1)];
% samples = (samples + add);
% p = polyfit(u, samples, 6);
% out = polyval(p,m);
% disp("wed");
% p
% out2 = p(1)*m.^6 + p(2)*m.^5 + p(3)*m.^4 + p(4)*m.^3 + p(5)*m.^2 + p(6)*m + p(7);
% out2 = out2 / div;
% out3 = out2.*rand(1,86400);
% figure(5)
% plot(u, samples/div); hold on;
% plot(m, out/div, "r-"); hold on;
% plot(m, out2, "bo"); hold on;
samples = get_samples("thu", scalar);
m = linspace(0, 24, 86400);
u = [0 , 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24];
samples = [samples, samples(1)];
samples = (samples + add);
p = polyfit(u, samples, 6);

fprintf(fileID, 'p3 = ');
fprintf(fileID,'%.20f, ',p);
fprintf(fileID, '\n');
out = polyval(p,m);
disp("thu");
out2 = p(1)*m.^6 + p(2)*m.^5 + p(3)*m.^4 + p(4)*m.^3 + p(5)*m.^2 + p(6)*m + p(7);
out2 = out2 / div;
out3 = out2.*rand(1,86400);
out4 = floor(out3);
figure(6)
plot(m, out/div, "r-"); hold on;
plot(m, out2, "bo"); hold on;
out5 = zeros(1,24);
for i=0:23
    for j=0:3599
        out5(i + 1) = out5(i + 1) + out4(i * 3600 + j + 1);
    end
end
w = 1:24;
figure(13)
plot(w, out5, "go"); hold on;
% samples = get_samples("thu", scalar);
% m = linspace(0, 86400, 86400);
% u = [0 , 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24];
% u = u.*3600; 
% samples = [samples, samples(1)];
% samples = (samples + add);
% p = polyfit(u, samples, 6);
% out = polyval(p,m);
% disp("thu");
% p
% out2 = p(1)*m.^6 + p(2)*m.^5 + p(3)*m.^4 + p(4)*m.^3 + p(5)*m.^2 + p(6)*m + p(7);
% out2 = out2 / div;
% out3 = out2.*rand(1,86400);
% figure(6)
% plot(u, samples/div); hold on;
% plot(m, out/div, "r-"); hold on;
% plot(m, out2, "bo"); hold on;
% 
samples = get_samples("fri", scalar);
m = linspace(0, 24, 86400);
u = [0 , 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24];
samples = [samples, samples(1)];
samples = (samples + add);
p = polyfit(u, samples, 6);

fprintf(fileID, 'p4 = ');
fprintf(fileID,'%.20f, ',p);
fprintf(fileID, '\n');
out = polyval(p,m);
disp("fri");
out2 = p(1)*m.^6 + p(2)*m.^5 + p(3)*m.^4 + p(4)*m.^3 + p(5)*m.^2 + p(6)*m + p(7);
out2 = out2 / div;
out3 = out2.*rand(1,86400);
out4 = floor(out3);
figure(7)
plot(m, out/div, "r-"); hold on;
plot(m, out2, "bo"); hold on;
out5 = zeros(1,24);
for i=0:23
    for j=0:3599
        out5(i + 1) = out5(i + 1) + out4(i * 3600 + j + 1);
    end
end
w = 1:24;
figure(14)
plot(w, out5, "go"); hold on;
% samples = get_samples("fri", scalar);
% m = linspace(0, 86400, 86400);
% u = [0 , 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24];
% u = u.*3600; 
% samples = [samples, samples(1)];
% samples = (samples + add);
% p = polyfit(u, samples, 6);
% out = polyval(p,m);
% disp("fri");
% p
% out2 = p(1)*m.^6 + p(2)*m.^5 + p(3)*m.^4 + p(4)*m.^3 + p(5)*m.^2 + p(6)*m + p(7);
% out2 = out2 / div;
% out3 = out2.*rand(1,86400);
% figure(7)
% plot(u, samples/div); hold on;
% plot(m, out/div, "r-"); hold on;
% plot(m, out2, "bo"); hold on;
% 
samples = get_samples("sat", scalar);
m = linspace(0, 24, 86400);
u = [0 , 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24];
samples = [samples, samples(1)];
samples = (samples + add);
p = polyfit(u, samples, 6);

fprintf(fileID, 'p5 = ');
fprintf(fileID,'%.20f, ',p);
fprintf(fileID, '\n');
out = polyval(p,m);
disp("sat");
out2 = p(1)*m.^6 + p(2)*m.^5 + p(3)*m.^4 + p(4)*m.^3 + p(5)*m.^2 + p(6)*m + p(7);
out2 = out2 / div;
out3 = out2.*rand(1,86400);
out4 = floor(out3);
figure(8)
plot(m, out/div, "r-"); hold on;
plot(m, out2, "bo"); hold on;
out5 = zeros(1,24);
for i=0:23
    for j=0:3599
        out5(i + 1) = out5(i + 1) + out4(i * 3600 + j + 1);
    end
end
w = 1:24;
figure(15)
plot(w, out5, "go"); hold on;
% samples = get_samples("sat", scalar);
% m = linspace(0, 86400, 86400);
% u = [0 , 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24];
% u = u.*3600; 
% samples = [samples, samples(1)];
% samples = (samples + add);
% p = polyfit(u, samples, 6);
% out = polyval(p,m);
% disp("sat");
% p
% out2 = p(1)*m.^6 + p(2)*m.^5 + p(3)*m.^4 + p(4)*m.^3 + p(5)*m.^2 + p(6)*m + p(7);
% out2 = out2 / div;
% out3 = out2.*rand(1,86400);
% figure(8)
% plot(u, samples/div); hold on;
% plot(m, out/div, "r-"); hold on;
% plot(m, out2, "bo"); hold on;
% 
samples = get_samples("sun", scalar);
m = linspace(0, 24, 86400);
u = [0 , 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24];
samples = [samples, samples(1)];
samples = (samples + add);
p = polyfit(u, samples, 6);

fprintf(fileID, 'p6 = ');
fprintf(fileID,'%.20f, ',p);
fclose(fileID);
out = polyval(p,m);
disp("sun");
out2 = p(1)*m.^6 + p(2)*m.^5 + p(3)*m.^4 + p(4)*m.^3 + p(5)*m.^2 + p(6)*m + p(7);
out2 = out2 / div;
out3 = out2.*rand(1,86400);
out4 = floor(out3);
figure(9)
plot(m, out/div, "r-"); hold on;
plot(m, out2, "r-"); hold on;
title('Maksymalna liczba aut na sekund�');
xlabel('godzina'); ylabel('liczba aut na sekund�');
legend('niedziela');
out5 = zeros(1,24);
for i=0:23
    for j=0:3599
        out5(i + 1) = out5(i + 1) + out4(i * 3600 + j + 1);
    end
end
w = 1:24;
figure(16)
bar(w, out5, 'b'); hold on;
title('Zsumowany godzinowy ruch samochod�w');
xlabel('godzina'); ylabel('liczba aut na godzin�');
legend('niedziela');
% samples = get_samples("sun", scalar);
% m = linspace(0, 86400, 86400);
% u = [0 , 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24];
% u = u.*3600; 
% samples = [samples, samples(1)];
% samples = (samples + add); %/500
% p = polyfit(u, samples, 6);
% out = polyval(p,m);
% disp("sun");
% p
% out2 = p(1)*m.^6 + p(2)*m.^5 + p(3)*m.^4 + p(4)*m.^3 + p(5)*m.^2 + p(6)*m + p(7);
% out2 = out2 / div;
% out3 = out2.*rand(1,86400);
% figure(9)
% plot(u, samples/div); hold on;
% plot(m, out/div, "r-"); hold on;
% plot(m, out2, "bo"); hold on;
% 
% 
% 


