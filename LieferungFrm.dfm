object LieferungForm: TLieferungForm
  Left = 361
  Top = 356
  Width = 835
  Height = 452
  Caption = 'Lieferung'
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
    Left = 297
    Top = 0
    Width = 5
    Height = 413
    Cursor = crHSplit
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 297
    Height = 413
    Align = alLeft
    TabOrder = 0
    object Panel3: TPanel
      Left = 1
      Top = 1
      Width = 295
      Height = 41
      Align = alTop
      TabOrder = 0
      object DBNavigator1: TDBNavigator
        Left = 8
        Top = 8
        Width = 240
        Height = 25
        DataSource = DataSourceLieferung
        TabOrder = 0
      end
    end
    object DBGridLieferung: TDBGrid
      Left = 1
      Top = 42
      Width = 295
      Height = 370
      Align = alClient
      DataSource = DataSourceLieferung
      TabOrder = 1
      TitleFont.Charset = DEFAULT_CHARSET
      TitleFont.Color = clWindowText
      TitleFont.Height = -11
      TitleFont.Name = 'MS Sans Serif'
      TitleFont.Style = []
    end
  end
  object Panel2: TPanel
    Left = 302
    Top = 0
    Width = 517
    Height = 413
    Align = alClient
    TabOrder = 1
    object Panel4: TPanel
      Left = 1
      Top = 1
      Width = 515
      Height = 41
      Align = alTop
      TabOrder = 0
      object DBNavigator2: TDBNavigator
        Left = 8
        Top = 8
        Width = 240
        Height = 25
        DataSource = DataSourceLieferungArtikel
        TabOrder = 0
      end
      object EditNumbers: TEdit
        Left = 264
        Top = 8
        Width = 225
        Height = 21
        TabOrder = 1
        OnEnter = EditNumbersEnter
        OnExit = EditNumbersExit
        OnKeyPress = EditNumbersKeyPress
      end
    end
    object DBGridLieferungArtikel: TDBGrid
      Left = 1
      Top = 42
      Width = 515
      Height = 370
      Align = alClient
      DataSource = DataSourceLieferungArtikel
      TabOrder = 1
      TitleFont.Charset = DEFAULT_CHARSET
      TitleFont.Color = clWindowText
      TitleFont.Height = -11
      TitleFont.Name = 'MS Sans Serif'
      TitleFont.Style = []
    end
  end
  object DataSourceLieferung: TDataSource
    DataSet = TableLieferung
    Left = 16
    Top = 64
  end
  object TableLieferung: TTable
    AfterInsert = TableLieferungAfterInsert
    AfterPost = TableLieferungAfterPost
    DatabaseName = 'kasseDB'
    IndexName = 'LIEFERUNG_DATUM_IDX'
    TableName = 'LIEFERUNG.db'
    Left = 112
    Top = 64
    object TableLieferungDATUM: TDateField
      DisplayLabel = 'Datum'
      FieldName = 'DATUM'
    end
    object TableLieferungINVENTUR: TBooleanField
      DisplayLabel = 'Inventur'
      FieldName = 'INVENTUR'
      ReadOnly = True
    end
    object TableLieferungID: TIntegerField
      FieldName = 'ID'
      Required = True
      Visible = False
    end
  end
  object DataSourceLieferungArtikel: TDataSource
    DataSet = TableLieferungArtikel
    Left = 334
    Top = 56
  end
  object TableLieferungArtikel: TTable
    AfterInsert = TableLieferungArtikelAfterInsert
    AfterPost = TableLieferungArtikelAfterPost
    DatabaseName = 'kasseDB'
    IndexName = 'LIEFERUNG'
    MasterFields = 'ID'
    MasterSource = DataSourceLieferung
    TableName = 'LIEFERUNG_ARTIKEL.db'
    Left = 454
    Top = 112
    object TableLieferungArtikelLIEFERUNG: TIntegerField
      FieldName = 'LIEFERUNG'
      Required = True
      Visible = False
    end
    object TableLieferungArtikelARTIKEL: TIntegerField
      FieldName = 'ARTIKEL'
      Required = True
      Visible = False
    end
    object TableLieferungArtikelMENGE: TIntegerField
      DisplayLabel = 'Menge'
      FieldName = 'MENGE'
      Required = True
    end
    object TableLieferungArtikelAT_BEZ: TStringField
      DisplayLabel = 'Artikel'
      FieldKind = fkLookup
      FieldName = 'AT_BEZ'
      LookupDataSet = TableArtikel
      LookupKeyFields = 'ID'
      LookupResultField = 'BEZEICHNUNG'
      KeyFields = 'ARTIKEL'
      Size = 64
      Lookup = True
    end
    object TableLieferungArtikelID: TIntegerField
      FieldName = 'ID'
      Required = True
      Visible = False
    end
  end
  object TableArtikel: TTable
    DatabaseName = 'kasseDB'
    TableName = 'ARTIKEL.DB'
    Left = 534
    Top = 56
  end
end
