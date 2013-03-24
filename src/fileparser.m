function [ info source data ] = fileparser( name )

fd = fopen(name, 'r');
info = zeros(1,4);
if fd ~= -1
	info = fscanf(fd, '%f', 4);
	source = fscanf(fd, '%s', 1);
	row = ceil(info(1)/info(2));
	column = ceil(info(3)/info(4));
	data = zeros(row, column);
	data = fscanf(fd, '%f', [column, row]);
	fclose(fd);
end
