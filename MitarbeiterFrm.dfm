object MitarbeiterForm: TMitarbeiterForm
  Left = 414
  Top = 450
  Width = 875
  Height = 406
  Caption = 'Mitarbeiter'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsMDIChild
  OldCreateOrder = False
  Position = poDefault
  Visible = True
  OnActivate = FormActivate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 859
    Height = 41
    Align = alTop
    TabOrder = 0
    object PaintBoxBarCode: TPaintBox
      Left = 256
      Top = 0
      Width = 417
      Height = 41
      OnPaint = PaintBoxBarCodePaint
    end
    object DBNavigator1: TDBNavigator
      Left = 8
      Top = 8
      Width = 240
      Height = 25
      DataSource = DataSource
      TabOrder = 0
    end
    object ButtonCreateEAN8: TButton
      Left = 680
      Top = 8
      Width = 75
      Height = 25
      Caption = 'EAN-8'
      TabOrder = 1
      OnClick = ButtonCreateEAN8Click
    end
    object ButtonPrint: TButton
      Left = 760
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Drucken'
      TabOrder = 2
      OnClick = ButtonPrintClick
    end
  end
  object DBGrid1: TDBGrid
    Left = 0
    Top = 41
    Width = 859
    Height = 326
    Align = alClient
    DataSource = DataSource
    TabOrder = 1
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
  end
  object DataSource: TDataSource
    DataSet = TableMitarbeiter
    Left = 48
    Top = 48
  end
  object TableMitarbeiter: TTable
    BeforeOpen = TableMitarbeiterBeforeOpen
    AfterInsert = TableMitarbeiterAfterInsert
    AfterPost = TableMitarbeiterAfterPost
    BeforeDelete = TableMitarbeiterBeforeDelete
    AfterScroll = TableMitarbeiterAfterScroll
    OnCalcFields = TableMitarbeiterCalcFields
    DatabaseName = 'kasseDB'
    IndexName = 'MA_NAME_IDX'
    TableName = 'MITARBEITER.DB'
    Left = 152
    Top = 56
    object TableMitarbeiterID: TIntegerField
      FieldName = 'ID'
      Required = True
      Visible = False
    end
    object TableMitarbeiterCODE: TStringField
      DisplayLabel = 'Code'
      DisplayWidth = 16
      FieldName = 'CODE'
      Size = 64
    end
    object TableMitarbeiterNAME: TStringField
      DisplayLabel = 'Name'
      DisplayWidth = 32
      FieldName = 'NAME'
      Required = True
      Size = 128
    end
    object TableMitarbeiterUSERNAME: TStringField
      DisplayLabel = 'OS Nutzername'
      DisplayWidth = 16
      FieldName = 'USERNAME'
      Size = 128
    end
    object TableMitarbeiterKONTOSTAND: TCurrencyField
      DisplayLabel = 'Anfangskontostand'
      FieldName = 'KONTOSTAND'
    end
    object TableMitarbeiterEND_KONTOSTAND: TCurrencyField
      DisplayLabel = 'Kontostand'
      FieldKind = fkCalculated
      FieldName = 'END_KONTOSTAND'
      Calculated = True
    end
    object TableMitarbeiterMAX_KREDIT: TCurrencyField
      DisplayLabel = 'Max. Kredit'
      FieldName = 'MAX_KREDIT'
    end
  end
  object QuerySoll: TQuery
    DatabaseName = 'kasseDB'
    SQL.Strings = (
      'select SOLL_KONTO, SUM( BETRAG ) as BETRAG'
      'from BUCHUNG'
      'group by SOLL_KONTO'
      'order by SOLL_KONTO')
    Left = 112
    Top = 176
    object QuerySollSOLL_KONTO: TIntegerField
      FieldName = 'SOLL_KONTO'
      Origin = 'KASSE."BUCHUNG.DB".SOLL_KONTO'
    end
    object QuerySollBETRAG: TCurrencyField
      FieldName = 'BETRAG'
      Origin = 'KASSE."BUCHUNG.DB".BETRAG'
    end
  end
  object QueryHaben: TQuery
    DatabaseName = 'kasseDB'
    SQL.Strings = (
      'select HABEN_KONTO, SUM( BETRAG ) as BETRAG'
      'from BUCHUNG'
      'group by HABEN_KONTO'
      'order by HABEN_KONTO')
    Left = 176
    Top = 176
    object QueryHabenHABEN_KONTO: TIntegerField
      FieldName = 'HABEN_KONTO'
      Origin = 'KASSE."BUCHUNG.DB".HABEN_KONTO'
    end
    object QueryHabenBETRAG: TCurrencyField
      FieldName = 'BETRAG'
      Origin = 'KASSE."BUCHUNG.DB".BETRAG'
    end
  end
  object QueryVerkauf: TQuery
    DatabaseName = 'kasseDB'
    SQL.Strings = (
      'select MITARBEITER, SUM( GESAMTPREIS ) as GESAMTPREIS'
      'from VERKAUF'
      'group by MITARBEITER'
      'order by MITARBEITER')
    Left = 256
    Top = 176
    object QueryVerkaufMITARBEITER: TIntegerField
      FieldName = 'MITARBEITER'
      Origin = 'KASSE."VERKAUF.DB".MITARBEITER'
    end
    object QueryVerkaufGESAMTPREIS: TCurrencyField
      FieldName = 'GESAMTPREIS'
      Origin = 'KASSE."VERKAUF.DB".GESAMTPREIS'
    end
  end
  object PrintDialog: TPrintDialog
    Left = 760
    Top = 32
  end
end
