object VerkaufForm: TVerkaufForm
  Left = 377
  Top = 238
  Width = 1019
  Height = 666
  Caption = 'Verkauf'
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
  object Splitter1: TSplitter
    Left = 337
    Top = 0
    Width = 5
    Height = 639
    Cursor = crHSplit
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 337
    Height = 639
    Align = alLeft
    TabOrder = 0
    object Panel3: TPanel
      Left = 1
      Top = 1
      Width = 335
      Height = 41
      Align = alTop
      TabOrder = 0
      object DBNavigator1: TDBNavigator
        Left = 8
        Top = 8
        Width = 240
        Height = 25
        DataSource = DataSourceVerkauf
        TabOrder = 0
        OnEnter = EditNumbersExit
      end
      object CheckBoxParty: TCheckBox
        Left = 256
        Top = 20
        Width = 57
        Height = 17
        Caption = 'Party'
        TabOrder = 1
        OnClick = CheckBoxPartyClick
      end
      object CheckBoxFilter: TCheckBox
        Left = 256
        Top = 2
        Width = 97
        Height = 17
        Caption = 'Filter'
        TabOrder = 2
        OnClick = CheckBoxFilterClick
      end
    end
    object DBGridVerkauf: TDBGrid
      Left = 1
      Top = 42
      Width = 335
      Height = 596
      Align = alClient
      DataSource = DataSourceVerkauf
      ReadOnly = True
      TabOrder = 1
      TitleFont.Charset = DEFAULT_CHARSET
      TitleFont.Color = clWindowText
      TitleFont.Height = -11
      TitleFont.Name = 'MS Sans Serif'
      TitleFont.Style = []
      OnDrawColumnCell = DBGridVerkaufDrawColumnCell
      OnEnter = EditNumbersExit
    end
  end
  object Panel2: TPanel
    Left = 342
    Top = 0
    Width = 669
    Height = 639
    Align = alClient
    TabOrder = 1
    object Panel4: TPanel
      Left = 1
      Top = 1
      Width = 667
      Height = 41
      Align = alTop
      TabOrder = 0
      object DBText1: TDBText
        Left = 504
        Top = 8
        Width = 161
        Height = 25
        DataField = 'GESAMTPREIS'
        DataSource = DataSourceVerkauf
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'Courier New'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object DBNavigator2: TDBNavigator
        Left = 8
        Top = 8
        Width = 240
        Height = 25
        DataSource = DataSourceVerkaufArtikel
        TabOrder = 0
        OnEnter = EditNumbersExit
      end
      object EditNumbers: TEdit
        Left = 264
        Top = 8
        Width = 137
        Height = 21
        TabOrder = 1
        OnChange = EditNumbersChange
        OnEnter = EditNumbersEnter
        OnExit = EditNumbersExit
        OnKeyPress = EditNumbersKeyPress
      end
      object CheckBoxKonto: TCheckBox
        Left = 408
        Top = 8
        Width = 97
        Height = 17
        Caption = 'Kontenwechsel'
        TabOrder = 2
        OnClick = CheckBoxKontoClick
      end
    end
    object DBGridVerkaufArtikel: TDBGrid
      Left = 1
      Top = 42
      Width = 667
      Height = 196
      Align = alClient
      DataSource = DataSourceVerkaufArtikel
      ReadOnly = True
      TabOrder = 1
      TitleFont.Charset = DEFAULT_CHARSET
      TitleFont.Color = clWindowText
      TitleFont.Height = -11
      TitleFont.Name = 'MS Sans Serif'
      TitleFont.Style = []
      OnEnter = EditNumbersExit
    end
    object PageControl: TPageControl
      Left = 1
      Top = 238
      Width = 667
      Height = 400
      ActivePage = TabSheetPrivate
      Align = alBottom
      MultiLine = True
      RaggedRight = True
      TabHeight = 30
      TabOrder = 2
      object TabSheetTop: TTabSheet
        Caption = 'Top'
      end
      object TabSheetPrivate: TTabSheet
        Caption = 'Privat'
        ImageIndex = 1
      end
    end
  end
  object DataSourceVerkauf: TDataSource
    DataSet = TableVerkauf
    Left = 32
    Top = 88
  end
  object TableVerkauf: TTable
    AfterOpen = TableVerkaufAfterOpen
    AfterInsert = TableVerkaufAfterInsert
    BeforeEdit = ProtectCompleted
    AfterPost = EditNumbersSetFocus
    AfterCancel = EditNumbersSetFocus
    BeforeDelete = ProtectCompleted
    AfterDelete = EditNumbersSetFocus
    AfterScroll = TableVerkaufAfterScroll
    AfterRefresh = EditNumbersSetFocus
    AutoRefresh = True
    DatabaseName = 'kasseDB'
    IndexName = 'VERKAUF_DATUM_IDX'
    TableName = 'VERKAUF'
    Left = 56
    Top = 152
    object TableVerkaufDATUM: TDateTimeField
      DisplayLabel = 'Datum/Zeit'
      FieldName = 'DATUM'
      Required = True
    end
    object TableVerkaufGESAMTPREIS: TCurrencyField
      DisplayLabel = 'Summe'
      FieldName = 'GESAMTPREIS'
      Required = True
    end
    object TableVerkaufMA_BEZ: TStringField
      DisplayLabel = 'Mitarbeiter'
      FieldKind = fkLookup
      FieldName = 'MA_BEZ'
      LookupDataSet = QueryMitarbeiter
      LookupKeyFields = 'ID'
      LookupResultField = 'NAME'
      KeyFields = 'MITARBEITER'
      Lookup = True
    end
    object TableVerkaufMITARBEITER: TIntegerField
      FieldName = 'MITARBEITER'
      Required = True
      Visible = False
    end
    object TableVerkaufPARTYMODE: TBooleanField
      FieldName = 'PARTYMODE'
      Visible = False
    end
    object TableVerkaufFERTIG: TBooleanField
      FieldName = 'FERTIG'
      Visible = False
    end
    object TableVerkaufID: TIntegerField
      FieldName = 'ID'
      Required = True
      Visible = False
    end
  end
  object DataSourceVerkaufArtikel: TDataSource
    DataSet = TableVerkaufArtikel
    Left = 374
    Top = 80
  end
  object TableVerkaufArtikel: TTable
    BeforeInsert = ProtectCompleted
    AfterInsert = TableVerkaufArtikelAfterInsert
    BeforeEdit = ProtectCompleted
    BeforePost = TableVerkaufArtikelBeforePost
    AfterPost = TableVerkaufArtikelAfterPost
    AfterCancel = EditNumbersSetFocus
    BeforeDelete = TableVerkaufArtikelBeforeDelete
    AfterDelete = EditNumbersSetFocus
    AfterScroll = TableVerkaufArtikelAfterScroll
    AfterRefresh = TableVerkaufArtikelAfterRefresh
    DatabaseName = 'kasseDB'
    IndexName = 'VERKAUF'
    MasterFields = 'ID'
    MasterSource = DataSourceVerkauf
    TableName = 'VERKAUF_ARTIKEL'
    Left = 374
    Top = 136
    object TableVerkaufArtikelVERKAUF: TIntegerField
      FieldName = 'VERKAUF'
      Required = True
      Visible = False
    end
    object TableVerkaufArtikelARTIKEL: TIntegerField
      FieldName = 'ARTIKEL'
      Required = True
      Visible = False
    end
    object TableVerkaufArtikelMENGE: TIntegerField
      DisplayLabel = 'Menge'
      FieldName = 'MENGE'
      Required = True
    end
    object TableVerkaufArtikelAT_BEZ: TStringField
      DisplayLabel = 'Artikel'
      FieldKind = fkLookup
      FieldName = 'AT_BEZ'
      LookupDataSet = QueryArtikel
      LookupKeyFields = 'ID'
      LookupResultField = 'BEZ_PREIS'
      KeyFields = 'ARTIKEL'
      Size = 64
      Lookup = True
    end
    object TableVerkaufArtikelID: TIntegerField
      FieldName = 'ID'
      Required = True
      Visible = False
    end
  end
  object QueryArtikel: TQuery
    OnCalcFields = QueryArtikelCalcFields
    DatabaseName = 'kasseDB'
    SQL.Strings = (
      'select ID, BEZEICHNUNG, VERKAUFSPREIS, PARTYPREIS'
      'from ARTIKEL'
      ' '
      ' ')
    Left = 654
    Top = 64
    object QueryArtikelID: TIntegerField
      FieldName = 'ID'
      Origin = 'KASSE."ARTIKEL.DB".ID'
    end
    object QueryArtikelBEZEICHNUNG: TStringField
      FieldName = 'BEZEICHNUNG'
      Origin = 'KASSE."ARTIKEL.DB".BEZEICHNUNG'
      Size = 255
    end
    object QueryArtikelVERKAUFSPREIS: TCurrencyField
      FieldName = 'VERKAUFSPREIS'
      Origin = 'KASSE."ARTIKEL.DB".VERKAUFSPREIS'
    end
    object QueryArtikelBEZ_PREIS: TStringField
      FieldKind = fkCalculated
      FieldName = 'BEZ_PREIS'
      Size = 128
      Calculated = True
    end
    object QueryArtikelPARTYPREIS: TCurrencyField
      FieldName = 'PARTYPREIS'
      Origin = 'KASSE."ARTIKEL.DB".PARTYPREIS'
    end
  end
  object QueryMitarbeiter: TQuery
    DatabaseName = 'kasseDB'
    SQL.Strings = (
      'select ID, NAME'
      'from MITARBEITER')
    Left = 168
    Top = 112
  end
  object QueryTopSeller: TQuery
    OnCalcFields = QueryArtikelCalcFields
    DatabaseName = 'kasseDB'
    SQL.Strings = (
      
        'select a.BEZEICHNUNG, a.GRUPPEN, sum(va.MENGE) as MENGE, min(c.C' +
        'ODE) as CODE,'
      #9'a.VERKAUFSPREIS, a.PARTYPREIS'
      'from artikel_code c'
      'join artikel a on a.id = c.artikel'
      'left outer join verkauf_artikel va on a.id = va.artikel'
      'group by a.BEZEICHNUNG, a.GRUPPEN, '
      #9'a.VERKAUFSPREIS, a.PARTYPREIS'
      'order by 3 desc'
      ''
      ' '
      ' '
      ' ')
    Left = 774
    Top = 64
    object QueryTopSellerBEZEICHNUNG: TStringField
      FieldName = 'BEZEICHNUNG'
      Origin = 'KASSEDB."artikel.DB".BEZEICHNUNG'
      Size = 255
    end
    object QueryTopSellerGRUPPEN: TStringField
      FieldName = 'GRUPPEN'
      Origin = 'KASSEDB."artikel.DB".GRUPPEN'
      Size = 255
    end
    object QueryTopSellerMENGE: TFloatField
      FieldName = 'MENGE'
      Origin = 'KASSEDB."VERKAUF_ARTIKEL.DB".MENGE'
    end
    object QueryTopSellerCODE: TStringField
      FieldName = 'CODE'
      Origin = 'KASSEDB."ARTIKEL_CODE.DB".CODE'
      Size = 64
    end
    object QueryTopSellerVERKAUFSPREIS: TCurrencyField
      FieldName = 'VERKAUFSPREIS'
    end
    object QueryTopSellerPARTYPREIS: TCurrencyField
      FieldName = 'PARTYPREIS'
    end
  end
  object QueryGroups: TQuery
    OnCalcFields = QueryArtikelCalcFields
    DatabaseName = 'kasseDB'
    SQL.Strings = (
      
        'select distinct GRUPPEN from ARTIKEL where GRUPPEN is not null o' +
        'rder by GRUPPEN')
    Left = 774
    Top = 144
    object QueryGroupsGRUPPEN: TStringField
      FieldName = 'GRUPPEN'
      Origin = 'KASSEDB."ARTIKEL.DB".GRUPPEN'
      Size = 255
    end
  end
  object TimerClientOrder: TTimer
    OnTimer = TimerClientOrderTimer
    Left = 120
    Top = 344
  end
end
