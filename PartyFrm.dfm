object PartyForm: TPartyForm
  Left = 475
  Top = 233
  Width = 826
  Height = 482
  Caption = 'Partyauswertung'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 810
    Height = 41
    Align = alTop
    TabOrder = 0
    object Label1: TLabel
      Left = 8
      Top = 8
      Width = 22
      Height = 13
      Caption = 'Von:'
    end
    object Label2: TLabel
      Left = 192
      Top = 8
      Width = 17
      Height = 13
      Caption = 'Bis:'
    end
    object ButtonQueryStart: TButton
      Left = 352
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Start'
      TabOrder = 2
      OnClick = ButtonQueryStartClick
    end
    object EditFromDate: TEdit
      Left = 40
      Top = 8
      Width = 121
      Height = 21
      TabOrder = 0
    end
    object EditToDate: TEdit
      Left = 216
      Top = 8
      Width = 121
      Height = 21
      TabOrder = 1
    end
    object ButtonExport: TButton
      Left = 432
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Export'
      TabOrder = 3
      OnClick = ButtonExportClick
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 415
    Width = 810
    Height = 28
    Align = alBottom
    TabOrder = 1
    object LabelResult: TLabel
      Left = 8
      Top = 8
      Width = 41
      Height = 13
      Caption = 'Ergebnis'
    end
  end
  object DBGrid1: TDBGrid
    Left = 0
    Top = 41
    Width = 810
    Height = 374
    Align = alClient
    DataSource = DataSourceParty
    TabOrder = 2
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
  end
  object DataSourceParty: TDataSource
    DataSet = QueryParty
    Left = 48
    Top = 88
  end
  object QueryParty: TQuery
    AfterOpen = QueryPartyAfterOpen
    DatabaseName = 'kasseDB'
    SQL.Strings = (
      'select a.BEZEICHNUNG,'
      #9'SUM( va.MENGE ) as MENGE,'
      #9'SUM( va.MENGE ) * a.PARTYPREIS as PARTYPREIS,'
      #9'SUM( va.MENGE ) * a.VERKAUFSPREIS as VERKAUFSPREIS'
      'from ARTIKEL a, VERKAUF_ARTIKEL va, VERKAUF v'
      'where v.PARTYMODE=true'
      'and v.DATUM >= :startDatum'
      'and v.Datum <= :endDatum'
      'and v.ID = va.VERKAUF'
      'and va.ARTIKEL = a.ID'
      'group by a.BEZEICHNUNG'
      'order by a.BEZEICHNUNG'
      ' '
      ' '
      ' '
      ' ')
    Left = 152
    Top = 96
    ParamData = <
      item
        DataType = ftDateTime
        Name = 'startDatum'
        ParamType = ptUnknown
      end
      item
        DataType = ftDateTime
        Name = 'endDatum'
        ParamType = ptUnknown
      end>
    object QueryPartyBEZEICHNUNG: TStringField
      DisplayLabel = 'Artikel'
      DisplayWidth = 64
      FieldName = 'BEZEICHNUNG'
      Origin = 'KASSE."ARTIKEL.DB".BEZEICHNUNG'
      Size = 255
    end
    object QueryPartyMENGE: TFloatField
      DisplayLabel = 'Menge'
      FieldName = 'MENGE'
      Origin = 'KASSE."VERKAUF_ARTIKEL.DB".MENGE'
    end
    object QueryPartyPARTYPREIS: TCurrencyField
      DisplayLabel = 'Partypreis'
      FieldName = 'PARTYPREIS'
      Origin = 'KASSE."VERKAUF_ARTIKEL.DB".MENGE'
    end
    object QueryPartyVERKAUFSPREIS: TCurrencyField
      DisplayLabel = 'VK'
      FieldName = 'VERKAUFSPREIS'
      Origin = 'KASSE."VERKAUF_ARTIKEL.DB".MENGE'
    end
  end
end
