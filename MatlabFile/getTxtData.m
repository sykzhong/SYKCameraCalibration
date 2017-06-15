function [ data] = getTxtData( filein )
%getTxtData 读取txt文件中的数组
%   filein: 被读取的txt文件
%   line：读取的行数
    fidin = fopen(filein,'r');
    data = textscan(fidin, '%f');
    data = cell2mat(data);
%     celldisp(data);
%     while ~feof(fidin)         %判断是否为文件末尾
%         dataout(nline) = fgetl(fidin);         %从文件读行
%         nline = nline+1;
%         if nline == line
%             break;
%         end
%     end
    fclose(fidin);
end

