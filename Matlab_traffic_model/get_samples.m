function [samples] = getsamples(day_of_week, scale)

switch day_of_week
    case "mon" %    0  2   4    6    8     10  12  14    16   18  20    22
        samples = [15, 10, 50, 120, 190, 140, 145, 160, 170, 200, 100, 60];
    case "tue"
        samples = [15, 10, 50, 120, 190, 130, 140, 165, 170, 210, 110, 60];
    case "wed"
        samples = [15, 10, 50, 120, 190, 130, 140, 165, 170, 210, 110, 60];
    case "thu"
        samples = [15, 10, 50, 120, 190, 130, 140, 165, 170, 210, 110, 60]; 
    case "fri"
        samples = [15, 10, 50, 120, 150, 130, 150, 185, 190, 210, 120, 70];        
    case "sat"
        samples = [45, 30, 35, 40,  95,  160, 190, 160, 150, 130, 80,  70];
    case "sun"
        samples = [45, 30, 25, 30,  80,  160, 190, 160, 170, 130, 80,  70];
    otherwise
        disp("ERROR, WRONG DAY");
        samples = zeros(12);
end
random = rand(12);
random = random / 20;
samples = samples + samples * random;
samples = samples * scale;
end

