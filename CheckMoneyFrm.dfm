object CheckMoneyForm: TCheckMoneyForm
  Left = 892
  Top = 490
  BorderStyle = bsDialog
  Caption = 'Kassenprüfung'
  ClientHeight = 135
  ClientWidth = 285
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 16
    Width = 58
    Height = 13
    Caption = 'Sollbestand:'
  end
  object Label2: TLabel
    Left = 16
    Top = 56
    Width = 52
    Height = 13
    Caption = 'Istbestand:'
  end
  object EditSollBestand: TEdit
    Left = 88
    Top = 16
    Width = 177
    Height = 21
    Enabled = False
    TabOrder = 0
  end
  object EditIstbestand: TEdit
    Left = 88
    Top = 56
    Width = 177
    Height = 21
    TabOrder = 1
    OnChange = EditIstbestandChange
    OnKeyPress = EditIstbestandKeyPress
  end
  object ButtonOK: TButton
    Left = 16
    Top = 96
    Width = 75
    Height = 25
    Caption = '&OK'
    Default = True
    TabOrder = 2
    OnClick = ButtonOKClick
  end
  object ButtonCancel: TButton
    Left = 104
    Top = 96
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'A&bbruch'
    ModalResult = 2
    TabOrder = 3
  end
end
