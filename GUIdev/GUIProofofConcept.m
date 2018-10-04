function varargout = GUIProofofConcept(varargin)
% GUIPROOFOFCONCEPT MATLAB code for GUIProofofConcept.fig
%      GUIPROOFOFCONCEPT, by itself, creates a new GUIPROOFOFCONCEPT or raises the existing
%      singleton*.
%
%      H = GUIPROOFOFCONCEPT returns the handle to a new GUIPROOFOFCONCEPT or the handle to
%      the existing singleton*.
%
%      GUIPROOFOFCONCEPT('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in GUIPROOFOFCONCEPT.M with the given input arguments.
%
%      GUIPROOFOFCONCEPT('Property','Value',...) creates a new GUIPROOFOFCONCEPT or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before GUIProofofConcept_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to GUIProofofConcept_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help GUIProofofConcept

% Last Modified by GUIDE v2.5 04-Oct-2018 17:04:07

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @GUIProofofConcept_OpeningFcn, ...
                   'gui_OutputFcn',  @GUIProofofConcept_OutputFcn, ...
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


% --- Executes just before GUIProofofConcept is made visible.
function GUIProofofConcept_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to GUIProofofConcept (see VARARGIN)

% Choose default command line output for GUIProofofConcept
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes GUIProofofConcept wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = GUIProofofConcept_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in ClearConnection.
function ClearConnection_Callback(hObject, eventdata, handles)
% hObject    handle to ClearConnection (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
clear handles.a;
clear handles.light;
guidata(hObject,handles);


% --- Executes on button press in InitializeButton.
function InitializeButton_Callback(hObject, eventdata, handles)
% hObject    handle to InitializeButton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
clear a;
try
    a = arduino();
    handles.a = a;
    configurePin(a,'D13','Unset');
    light = readDigitalPin(a,'D13');
    configurePin(a,'D13','Unset');
    handles.light = light;
    guidata(hObject,handles);
catch
    clear a;
    handles.a = 'Unsuccessful';
end

% --- Executes on button press in ToggleButton.
function ToggleButton_Callback(hObject, eventdata, handles)
% hObject    handle to ToggleButton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
try
    a = handles.a;
    light = handles.light;
    if light
        writeDigitalPin(a,'D13',0);
        light = 0;
    else
        writeDigitalPin(a,'D13',1);
        light = 1;
    end
    handles.light = light;
    guidata(hObject,handles);
catch
    clear a;
    clear handles.a;
end
