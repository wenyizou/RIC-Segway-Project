% function for just check the motor status
% parameter:  none
%
% function example: check_motorstus();
%
% return : the motor.xml file inside the controller, which contain the
% status information
%
function motor_status=check_motorstus()
msurl='http://192.168.1.3/motor.xml';% motor status url
motor_status = urlread(msurl);  % request motor status back to Matlab
end