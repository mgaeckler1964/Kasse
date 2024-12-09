object KasseForm: TKasseForm
  Left = 385
  Top = 172
  Width = 454
  Height = 320
  Caption = 'GAK Kassenterminal'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsMDIForm
  Menu = MainMenu
  OldCreateOrder = False
  Position = poDefault
  Visible = True
  WindowState = wsMaximized
  OnActivate = FormActivate
  OnClose = FormClose
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object MainMenu: TMainMenu
    Left = 32
    Top = 32
    object Datei1: TMenuItem
      Caption = '&Datei'
      object Configuration: TMenuItem
        Caption = '&Einrichtung...'
        OnClick = ConfigurationClick
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object ArtikelExport1: TMenuItem
        Caption = '&Artikel Export...'
        OnClick = ArtikelExport1Click
      end
      object ArtikelImport1: TMenuItem
        Caption = 'Artikel &Import...'
        OnClick = ArtikelImport1Click
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object Ende1: TMenuItem
        Caption = '&Ende'
        OnClick = Ende1Click
      end
    end
    object Aufgaben1: TMenuItem
      Caption = '&Aufgaben'
      object Verkauf1: TMenuItem
        Caption = '&Verkauf'
        ShortCut = 112
        OnClick = Verkauf1Click
      end
      object Buchungen1: TMenuItem
        Caption = '&Buchungen'
        ShortCut = 113
        OnClick = Buchungen1Click
      end
      object Mitarbeiter1: TMenuItem
        Caption = '&Mitarbeiter'
        ShortCut = 114
        OnClick = Mitarbeiter1Click
      end
      object Artikel1: TMenuItem
        Caption = '&Artikel'
        ShortCut = 115
        OnClick = Artikel1Click
      end
      object Lieferungen1: TMenuItem
        Caption = '&Lieferungen'
        ShortCut = 116
        OnClick = Lieferungen1Click
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object NeuerVerkauf1: TMenuItem
        Caption = '&Neuer Verkauf'
        ShortCut = 117
        OnClick = Verkauf1Click
      end
      object PartyAuswertung: TMenuItem
        Caption = '&Party Auswertung'
        ShortCut = 118
        OnClick = PartyAuswertungClick
      end
      object N4: TMenuItem
        Caption = '-'
      end
      object Kassenprfung: TMenuItem
        Caption = '&Kassenprüfung...'
        OnClick = KassenprfungClick
      end
      object Inventur: TMenuItem
        Caption = '&Inventur...'
        OnClick = InventurClick
      end
      object Abschluss: TMenuItem
        Caption = 'Ab&schluß...'
        OnClick = AbschlussClick
      end
    end
    object MenuBerichte: TMenuItem
      AutoHotkeys = maManual
      Caption = '&Berichte'
    end
    object Info: TMenuItem
      Caption = '&Info!'
      OnClick = InfoClick
    end
  end
  object theDatabase: TDatabase
    AliasName = 'KASSE'
    DatabaseName = 'kasseDB'
    SessionName = 'Default'
    Left = 112
    Top = 24
  end
end
