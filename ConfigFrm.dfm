object ConfigForm: TConfigForm
  Left = 379
  Top = 350
  Width = 619
  Height = 284
  Caption = 'Einrichtung'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 62
    Height = 13
    Caption = 'Organisation:'
  end
  object Label2: TLabel
    Left = 8
    Top = 48
    Width = 42
    Height = 13
    Caption = 'Berichte:'
  end
  object Label3: TLabel
    Left = 8
    Top = 104
    Width = 19
    Height = 13
    Caption = 'Port'
  end
  object Label4: TLabel
    Left = 8
    Top = 144
    Width = 45
    Height = 13
    Caption = 'Kennwort'
  end
  object EditOrganisation: TEdit
    Left = 88
    Top = 8
    Width = 513
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 0
  end
  object ButtonOK: TButton
    Left = 8
    Top = 216
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    TabOrder = 6
    OnClick = ButtonOKClick
  end
  object ButtonCancel: TButton
    Left = 88
    Top = 216
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Abbruch'
    ModalResult = 2
    TabOrder = 7
  end
  object EditReports: TEdit
    Left = 88
    Top = 40
    Width = 513
    Height = 21
    TabOrder = 1
  end
  object CheckBoxEnableServer: TCheckBox
    Left = 88
    Top = 80
    Width = 57
    Height = 17
    Caption = 'Server'
    TabOrder = 2
  end
  object EditServerPort: TEdit
    Left = 88
    Top = 104
    Width = 121
    Height = 21
    TabOrder = 3
  end
  object EditPassword1: TEdit
    Left = 88
    Top = 144
    Width = 121
    Height = 21
    PasswordChar = '*'
    TabOrder = 4
  end
  object EditPassword2: TEdit
    Left = 88
    Top = 176
    Width = 121
    Height = 21
    PasswordChar = '*'
    TabOrder = 5
  end
end
