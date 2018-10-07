function varargout = command_engine_comp(varargin)
% COMMAND_ENGINE_COMP MATLAB code for command_engine_comp.fig
%      COMMAND_ENGINE_COMP, by itself, creates a new COMMAND_ENGINE_COMP or raises the existing
%      singleton*.
%
%      H = COMMAND_ENGINE_COMP returns the handle to a new COMMAND_ENGINE_COMP or the handle to
%      the existing singleton*.
%
%      COMMAND_ENGINE_COMP('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in COMMAND_ENGINE_COMP.M with the given input arguments.
%
%      COMMAND_ENGINE_COMP('Property','Value',...) creates a new COMMAND_ENGINE_COMP or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before command_engine_comp_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to command_engine_comp_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help command_engine_comp

% Last Modified by GUIDE v2.5 07-Oct-2018 10:09:17

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @command_engine_comp_OpeningFcn, ...
                   'gui_OutputFcn',  @command_engine_comp_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before command_engine_comp is made visible.
function command_engine_comp_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to command_engine_comp (see VARARGIN)

% Choose default command line output for command_engine_comp
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes command_engine_comp wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = command_engine_comp_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on slider movement.
function starter_motor_throttle_slider_Callback(hObject, eventdata, handles)
% hObject    handle to starter_motor_throttle_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider


% --- Executes during object creation, after setting all properties.
function starter_motor_throttle_slider_CreateFcn(hObject, eventdata, handles)
% hObject    handle to starter_motor_throttle_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end


% --- Executes on button press in starter_motor_power_button.
function starter_motor_power_button_Callback(hObject, eventdata, handles)
% hObject    handle to starter_motor_power_button (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of starter_motor_power_button


% --- Executes on button press in solinoid_valve_power_button.
function solinoid_valve_power_button_Callback(hObject, eventdata, handles)
% hObject    handle to solinoid_valve_power_button (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of solinoid_valve_power_button


% --- Executes on button press in fuel_pump_power_button.
function fuel_pump_power_button_Callback(hObject, eventdata, handles)
% hObject    handle to fuel_pump_power_button (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of fuel_pump_power_button


% --- Executes on slider movement.
function fuel_pump_throttle_slider_Callback(hObject, eventdata, handles)
% hObject    handle to fuel_pump_throttle_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider


% --- Executes during object creation, after setting all properties.
function fuel_pump_throttle_slider_CreateFcn(hObject, eventdata, handles)
% hObject    handle to fuel_pump_throttle_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end
