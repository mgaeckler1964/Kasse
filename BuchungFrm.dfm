object BuchungenForm: TBuchungenForm
  Left = 537
  Top = 384
  Width = 564
  Height = 510
  Caption = 'Buchungen'
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
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 548
    Height = 41
    Align = alTop
    TabOrder = 0
    object DBNavigator1: TDBNavigator
      Left = 8
      Top = 8
      Width = 240
      Height = 25
      DataSource = DataSourceBuchung
      TabOrder = 0
    end
  end
  object DBGrid1: TDBGrid
    Left = 0
    Top = 41
    Width = 548
    Height = 430
    Align = alClient
    DataSource = DataSourceBuchung
    TabOrder = 1
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
  end
  object DataSourceBuchung: TDataSource
    DataSet = TableBuchung
    Left = 40
    Top = 56
  end
  object TableBuchung: TTable
    AfterInsert = TableBuchungAfterInsert
    BeforeEdit = TableBuchungBeforePost
    BeforePost = TableBuchungBeforePost
    AfterPost = TableBuchungAfterPost
    BeforeDelete = TableBuchungBeforePost
    AfterDelete = TableBuchungAfterPost
    DatabaseName = 'kasseDB'
    Filtered = True
    OnFilterRecord = TableBuchungFilterRecord
    IndexName = 'BUCHUNG_DATUM_IDX'
    TableName = 'BUCHUNG.DB'
    Left = 152
    Top = 56
    object TableBuchungDATUM: TDateTimeField
      DisplayLabel = 'Datum'
      FieldName = 'DATUM'
      Required = True
    end
    object TableBuchungSOLL_KONTO: TIntegerField
      FieldName = 'SOLL_KONTO'
      Required = True
      Visible = False
    end
    object TableBuchungHABEN_KONTO: TIntegerField
      FieldName = 'HABEN_KONTO'
      Required = True
      Visible = False
    end
    object TableBuchungSOLL_KONTO_BEZ: TStringField
      DisplayLabel = 'Soll Konto'
      FieldKind = fkLookup
      FieldName = 'SOLL_KONTO_BEZ'
      LookupDataSet = QueryMitarbeiter
      LookupKeyFields = 'ID'
      LookupResultField = 'NAME'
      KeyFields = 'SOLL_KONTO'
      Size = 64
      Lookup = True
    end
    object TableBuchungHABEN_KONTO_BEZ: TStringField
      DisplayLabel = 'Haben Konto'
      FieldKind = fkLookup
      FieldName = 'HABEN_KONTO_BEZ'
      LookupDataSet = QueryMitarbeiter
      LookupKeyFields = 'ID'
      LookupResultField = 'NAME'
      KeyFields = 'HABEN_KONTO'
      Size = 64
      Lookup = True
    end
    object TableBuchungBETRAG: TCurrencyField
      DisplayLabel = 'Betrag'
      FieldName = 'BETRAG'
      Required = True
    end
    object TableBuchungID: TIntegerField
      FieldName = 'ID'
      Required = True
      Visible = False
    end
  end
  object QueryMitarbeiter: TQuery
    DatabaseName = 'kasseDB'
    SQL.Strings = (
      'select ID, NAME'
      'from MITARBEITER'
      'order by name')
    Left = 264
    Top = 64
    object QueryMitarbeiterID: TIntegerField
      FieldName = 'ID'
      Origin = 'KASSE."MITARBEITER.DB".ID'
    end
    object QueryMitarbeiterNAME: TStringField
      FieldName = 'NAME'
      Origin = 'KASSE."MITARBEITER.DB".NAME'
      Size = 128
    end
  end
end
