object ArtikelForm: TArtikelForm
  Left = 409
  Top = 503
  Width = 1009
  Height = 427
  Caption = 'Artikel'
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
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object DBGridArtikel: TDBGrid
    Left = 0
    Top = 41
    Width = 832
    Height = 347
    Align = alClient
    DataSource = DataSourceArtikel
    TabOrder = 0
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 993
    Height = 41
    Align = alTop
    TabOrder = 1
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
      DataSource = DataSourceArtikel
      TabOrder = 0
    end
    object ButtonCreateEAN13: TButton
      Left = 680
      Top = 8
      Width = 75
      Height = 25
      Caption = 'EAN-13'
      TabOrder = 1
      OnClick = ButtonCreateEAN13Click
    end
    object ButtonPrint: TButton
      Left = 760
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Drucken'
      PopupMenu = PopupMenuPrint
      TabOrder = 2
      OnClick = ButtonPrintClick
    end
    object DBNavigatorArtikelCode: TDBNavigator
      Left = 848
      Top = 8
      Width = 144
      Height = 25
      DataSource = DataSourceArtikelCode
      VisibleButtons = [nbInsert, nbDelete, nbEdit, nbPost, nbCancel, nbRefresh]
      Anchors = [akTop, akRight]
      TabOrder = 3
    end
  end
  object DBGridBarCodes: TDBGrid
    Left = 832
    Top = 41
    Width = 161
    Height = 347
    Align = alRight
    DataSource = DataSourceArtikelCode
    TabOrder = 2
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
  end
  object TableArtikel: TTable
    AfterInsert = TableArtikelAfterInsert
    AfterPost = TableArtikelAfterPost
    BeforeDelete = TableArtikelBeforeDelete
    OnCalcFields = TableArtikelCalcFields
    DatabaseName = 'kasseDB'
    IndexName = 'ARTIKEL_NAME_IDX'
    TableName = 'ARTIKEL'
    Left = 24
    Top = 56
    object TableArtikelBEZEICHNUNG: TStringField
      DisplayLabel = 'Bezeichnung'
      DisplayWidth = 32
      FieldName = 'BEZEICHNUNG'
      Required = True
      Size = 255
    end
    object TableArtikelGRUPPEN: TStringField
      DisplayLabel = 'Gruppe'
      DisplayWidth = 16
      FieldName = 'GRUPPEN'
      Size = 255
    end
    object TableArtikelBESTAND: TIntegerField
      DisplayLabel = 'Anfangs Bestand'
      FieldName = 'BESTAND'
    end
    object TableArtikelEINHEIT: TIntegerField
      DisplayLabel = 'VE'
      DisplayWidth = 2
      FieldName = 'EINHEIT'
    end
    object TableArtikelEINKAUFSPREIS: TCurrencyField
      DisplayLabel = 'EK'
      DisplayWidth = 6
      FieldName = 'EINKAUFSPREIS'
    end
    object TableArtikelVERKAUFSPREIS: TCurrencyField
      DisplayLabel = 'VK'
      DisplayWidth = 6
      FieldName = 'VERKAUFSPREIS'
    end
    object TableArtikelPARTYPREIS: TCurrencyField
      DisplayLabel = 'Partypreis'
      DisplayWidth = 6
      FieldName = 'PARTYPREIS'
    end
    object TableArtikelSOLL_BESTAND: TIntegerField
      DisplayLabel = 'Soll Bestand'
      FieldKind = fkCalculated
      FieldName = 'SOLL_BESTAND'
      Calculated = True
    end
    object TableArtikelIST_BESTAND: TIntegerField
      DisplayLabel = 'Ist Bestand'
      FieldName = 'IST_BESTAND'
    end
    object TableArtikelVERKAUF: TFloatField
      DisplayLabel = 'Verk./Tag'
      FieldKind = fkCalculated
      FieldName = 'VERKAUF'
      DisplayFormat = '#.##'
      Calculated = True
    end
    object TableArtikelRESTZEIT: TFloatField
      DisplayLabel = 'Restzeit'
      FieldKind = fkCalculated
      FieldName = 'RESTZEIT'
      DisplayFormat = '#.##'
      Calculated = True
    end
    object TableArtikelSTART: TDateField
      DisplayLabel = 'Start'
      FieldKind = fkCalculated
      FieldName = 'START'
      Calculated = True
    end
    object TableArtikelID: TIntegerField
      FieldName = 'ID'
      Required = True
      Visible = False
    end
  end
  object DataSourceArtikel: TDataSource
    DataSet = TableArtikel
    Left = 272
    Top = 64
  end
  object QueryVerkauf: TQuery
    DatabaseName = 'kasseDB'
    SQL.Strings = (
      
        'select va.ARTIKEL, sum( va.MENGE ) as MENGE, min( v.DATUM ) as M' +
        'IN_DATUM'
      'from VERKAUF_ARTIKEL va, VERKAUF v'
      'where va.VERKAUF = v.ID'
      'group by va.artikel'
      'order by va.artikel'
      ' '
      '')
    Left = 88
    Top = 304
    object QueryVerkaufARTIKEL: TIntegerField
      FieldName = 'ARTIKEL'
      Origin = 'KASSE."VERKAUF_ARTIKEL.DB".ARTIKEL'
    end
    object QueryVerkaufMENGE: TFloatField
      FieldName = 'MENGE'
      Origin = 'KASSE."VERKAUF_ARTIKEL.DB".MENGE'
    end
    object QueryVerkaufMIN_DATUM: TDateTimeField
      FieldName = 'MIN_DATUM'
      Origin = 'KASSEDB."VERKAUF.DB".DATUM'
    end
  end
  object QueryLieferung: TQuery
    DatabaseName = 'kasseDB'
    SQL.Strings = (
      'select ARTIKEL, sum( menge ) as MENGE'
      'from lieferung_artikel'
      'group by artikel'
      'order by artikel')
    Left = 216
    Top = 248
    object QueryLieferungARTIKEL: TIntegerField
      FieldName = 'ARTIKEL'
      Origin = 'KASSE."LIEFERUNG_ARTIKEL.DB".ARTIKEL'
    end
    object QueryLieferungMENGE: TFloatField
      FieldName = 'MENGE'
      Origin = 'KASSE."LIEFERUNG_ARTIKEL.DB".MENGE'
    end
  end
  object PrintDialog: TPrintDialog
    Left = 760
    Top = 32
  end
  object PopupMenuPrint: TPopupMenu
    Left = 760
    Top = 152
    object MenuPrintAktuelles: TMenuItem
      Caption = 'Aktuelles'
      OnClick = ButtonPrintClick
    end
    object MenuAktuelleGruppe: TMenuItem
      Caption = 'Aktuelle Gruppe'
      OnClick = ButtonPrintClick
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object MenuPrintBestand: TMenuItem
      Caption = 'Bestand'
      OnClick = ButtonPrintClick
    end
    object MenuPrintPrivate: TMenuItem
      Caption = 'Privat'
      OnClick = ButtonPrintClick
    end
    object MenuPrintBestandUndPrivat: TMenuItem
      Caption = 'Bestand und Privat'
      OnClick = ButtonPrintClick
    end
  end
  object TableArtikelCode: TTable
    BeforeInsert = TableArtikelCodeBeforeInsert
    AfterInsert = TableArtikelCodeAfterInsert
    AfterPost = TableArtikelCodeAfterPost
    AfterScroll = TableArtikelCodeAfterScroll
    DatabaseName = 'kasseDB'
    IndexName = 'ARTIKEL'
    MasterFields = 'ID'
    MasterSource = DataSourceArtikel
    TableName = 'ARTIKEL_CODE'
    UpdateMode = upWhereKeyOnly
    Left = 152
    Top = 56
    object TableArtikelCodeID: TIntegerField
      FieldName = 'ID'
      Required = True
      Visible = False
    end
    object TableArtikelCodeCODE: TStringField
      DisplayLabel = 'Code'
      FieldName = 'CODE'
      Required = True
      Size = 64
    end
    object TableArtikelCodeARTIKEL: TIntegerField
      FieldName = 'ARTIKEL'
      Required = True
      Visible = False
    end
  end
  object DataSourceArtikelCode: TDataSource
    DataSet = TableArtikelCode
    Left = 160
    Top = 136
  end
  object QueryArtikelCode: TQuery
    DatabaseName = 'kasseDB'
    SQL.Strings = (
      'select ARTIKEL from ARTIKEL_CODE'
      'where CODE = :theCode')
    Left = 336
    Top = 104
    ParamData = <
      item
        DataType = ftString
        Name = 'theCode'
        ParamType = ptInput
      end>
    object QueryArtikelCodeARTIKEL: TIntegerField
      FieldName = 'ARTIKEL'
      Origin = 'KASSEDB."ARTIKEL_CODE.DB".ARTIKEL'
    end
  end
  object OpenDialog: TOpenDialog
    DefaultExt = 'CSV'
    Filter = 'CSV-Dateien (*.csv)|*.csv'
    Options = [ofHideReadOnly, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Title = 'Artikel Import...'
    Left = 528
    Top = 216
  end
  object SaveDialog: TSaveDialog
    DefaultExt = 'CSV'
    Filter = 'CSV-Dateien (*.csv)|*.csv'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Title = 'Artikel Export...'
    Left = 592
    Top = 216
  end
  object QueryArtikelImport: TQuery
    DatabaseName = 'kasseDB'
    SQL.Strings = (
      'update artikel'
      'set verkaufspreis = :newPrice,'
      #9'partyPreis = :newPartyPrice'
      'where id = :theId')
    Left = 32
    Top = 128
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'newPrice'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'newPartyPrice'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'theId'
        ParamType = ptUnknown
      end>
  end
end
