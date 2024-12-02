object CompleteForm: TCompleteForm
  Left = 402
  Top = 292
  Width = 259
  Height = 174
  Caption = 'Abschluß'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnActivate = FormActivate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object LabelSumme: TLabel
    Left = 8
    Top = 8
    Width = 78
    Height = 24
    Caption = 'Summe:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object LabelBack: TLabel
    Left = 8
    Top = 72
    Width = 72
    Height = 24
    Caption = 'Zurück:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label3: TLabel
    Left = 8
    Top = 40
    Width = 94
    Height = 24
    Caption = 'Gegeben:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object EditGiven: TEdit
    Left = 120
    Top = 40
    Width = 121
    Height = 32
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    OnChange = EditGivenChange
  end
  object ButtonOK: TButton
    Left = 8
    Top = 112
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 1
    OnEnter = NoButtonFocus
  end
  object ButtonCancel: TButton
    Left = 96
    Top = 112
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Abbruch'
    ModalResult = 2
    TabOrder = 2
    OnEnter = NoButtonFocus
  end
end
