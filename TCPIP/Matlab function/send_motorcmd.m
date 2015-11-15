% function [varargout]=send_motorcmd(motornum,motorcmd,operand,run_repeat)
% function for send the command to motor and get the feedback of
% motor status
% INPUT PARAMETERS:  (all input with char or string format)
% ----------------------------------
%   motornum  :          motor number
%   motorcmd  :          motor command char
%   operand   :          corresponding operand
%   run_repeat:          'R' means no repeat, 'X' means repeat
%
% function example: send_motorcmd('1','Z','10000','R');
%
% OUTPUT PARAMETER :
% ----------------------------------
%   only 1 output : output the motor.xml file
%
%   4 output parameter:
%        1st para:        sensor_data : 1*8 size array, store these sensors' value in sequence:
%                         1:gyro_x, 2:gyro_y, 3:gyro_z, 4:accel_x, 5:accel_y, 6:accel_z,
%                         7:anterior, 8:posterior
%        2 to 4 para:     feedback string of command_response, motor_position, query_response
%
%   11 output parameter:
%                         1st to 8th para: store these sensors' value in sequence:
%                         1:gyro_x, 2:gyro_y, 3:gyro_z, 4:accel_x, 5:accel_y, 6:accel_z,
%                         7:anterior, 8:posterior
%        9 to 11 para:    feedback string of command_response, motor_position, query_response
%
% EXAMPLE:
% -----------------------------------
%   motor_status = send_motorcmd('1','A','00000','R');
%   
%   [sensor_data, command_response, motor_position, query_response] = send_motorcmd('1','A','00000','R');
%
%   [gyro_x, gyro_y, gyro_z, accel_x, accel_y, accel_z, anterior,
%   posterior, command_response, motor_position, query_response] = send_motorcmd('1','A','00000','R');
%
% AUTHOR:   Wenyi Zou
% DATE  :   2014/8/19
% -----------------------------------
function [varargout]=send_motorcmd(motornum,motorcmd,operand,run_repeat)
mcurl='http://192.168.1.3/mc.cgi';  % motor command url
msurl='http://192.168.1.3/motor.xml';% motor status url
str=urlread(mcurl,'Get',{'motornum',motornum,'motorcmd',motorcmd,'operand',...
    operand,'run_repeat',run_repeat});  % send the command u want
str=urlread(mcurl,'Get',{'motornum',motornum,'motorcmd','?','operand',...
    '0','run_repeat',''});  % send query command to update the motor position
test = 1;
if (nargout == 1)
    varargout{1} = urlread(msurl);  % request motor status back to Matlab,
    return;                     % only 1 output parameter, that is motor.xml file, no need to do the number substract
else
    motor_status =  urlread(msurl);
end

if (nargout == 4)
    varargout{1} = zeros(1,8);         % first one store the sensor data
end



%%  substract the sensor information from xml file
for i=1:1:11
    switch(i)
        case 1
            str_id='gyro_x';
        case 2
            str_id='gyro_y';
        case 3
            str_id='gyro_z';
        case 4
            str_id='accel_x';
        case 5
            str_id='accel_y';
        case 6
            str_id='accel_z';
        case 7
            str_id='anterior';
        case 8
            str_id='posterior';
        case 9
            str_id='command_response';
        case 10
            str_id='motor_position';
        case 11
            str_id='query_response';
    end
    str_pos = strfind(motor_status,str_id);
    str_pos = str_pos + length(str_id) + 1;      % store the start pos of number, +1 is for the extra :
    str_pos2 = str_pos;         % store the end pos of number
    while (motor_status(str_pos2)~=' ')
        str_pos2 = str_pos2 + 1;    % stop when find blank
    end
    if (i < 9)
        if (nargout == 4)
            varargout{1}(i) = str2num(motor_status(str_pos:str_pos2-1));    % store sensor data in first output
        elseif(nargout == 11)
            %eval([str_id,'= str2num(motor_status(str_pos:str_pos2-1))']);    % convert from string to number
            varargout{i} = str2num(motor_status(str_pos:str_pos2-1));   % store sensor data sequently
        end
    else
        if (motor_status(str_pos) == 'L')
            if (nargout == 4)
                varargout{i-7}='Lost Connection';        % double ' can make ' show in string
            elseif (nargout == 11)
                varargout{i}='Lost Connection';
            end
        else
            if (nargout == 4)
                varargout{i-7}=motor_status(str_pos:str_pos2-1);        % double ' can make ' show in string
            elseif (nargout == 11)
                varargout{i}=motor_status(str_pos:str_pos2-1);
            end
        end
    end
end


%%



end

