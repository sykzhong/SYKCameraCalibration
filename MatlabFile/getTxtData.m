function [ data] = getTxtData( filein )
%getTxtData ��ȡtxt�ļ��е�����
%   filein: ����ȡ��txt�ļ�
%   line����ȡ������
    fidin = fopen(filein,'r');
    data = textscan(fidin, '%f');
    data = cell2mat(data);
%     celldisp(data);
%     while ~feof(fidin)         %�ж��Ƿ�Ϊ�ļ�ĩβ
%         dataout(nline) = fgetl(fidin);         %���ļ�����
%         nline = nline+1;
%         if nline == line
%             break;
%         end
%     end
    fclose(fidin);
end

