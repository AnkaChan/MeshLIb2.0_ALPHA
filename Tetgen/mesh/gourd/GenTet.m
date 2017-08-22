tet = importdata('./data/gourd.1.ele');
vert = importdata('./data/gourd.1.node');
fileID = fopen('eight.t', 'w');
[tetm, tetn] = size(tet);
[vertm, vertn] = size(vert);
for i = 1:vertm
    fprintf(fileID, 'Vertex %d %f %f %f\n', i, vert(i,2), vert(i,3), vert(i,4));
end

for j = 1:tetm
    fprintf(fileID, 'Tet %d %d %d %d %d\n', j, tet(j,2)+1, tet(j,3)+1, tet(j,4)+1, tet(j,5)+1);
end

fclose(fileID);