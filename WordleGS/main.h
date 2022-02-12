/*
 * MIT License
 *
 * Copyright (c) 2022 Dan Malec
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _____PROJECTNAMEASIDENTIFIER________FILEBASENAMEASIDENTIFIER_____
#define _____PROJECTNAMEASIDENTIFIER________FILEBASENAMEASIDENTIFIER_____

/* *****************************************************************************
 * Menu
 * *****************************************************************************/

#define rez_menubar_Main                           1


/* Generic Menu Items */

#define rez_menuitem_Divider                     999
#define rez_menuitem_DividerId                   999
#define rez_menuitem_DividerTitle                999

/* Apple Menu */

#define rez_menu_Apple                             1
#define rez_menu_AppleId                           1
#define rez_menu_AppleTitle                        1

#define rez_menuitem_About                       257
#define rez_menuitem_AboutId                     257
#define rez_menuitem_AboutTitle                  257

#define rez_menuitem_Help                        300
#define rez_menuitem_HelpId                      300
#define rez_menuitem_HelpTitle                   300


/* File Menu */

#define rez_menu_File                              2
#define rez_menu_FileId                            2
#define rez_menu_FileTitle                         2

#define rez_menuitem_NewGame                     255
#define rez_menuitem_NewGameId                   255
#define rez_menuitem_NewGameTitle                255

#define rez_menuitem_Quit                        256
#define rez_menuitem_QuitId                      256
#define rez_menuitem_QuitTitle                   256


/* Edit Menu */

#define rez_menu_Edit                              3
#define rez_menu_EditId                            3
#define rez_menu_EditTitle                         3

#define rez_menuitem_Undo                        250
#define rez_menuitem_UndoId                      250
#define rez_menuitem_UndoTitle                   250

#define rez_menuitem_Cut                         251
#define rez_menuitem_CutId                       251
#define rez_menuitem_CutTitle                    251

#define rez_menuitem_Copy                        252
#define rez_menuitem_CopyId                      252
#define rez_menuitem_CopyTitle                   252

#define rez_menuitem_Paste                       253
#define rez_menuitem_PasteId                     253
#define rez_menuitem_PasteTitle                  253

#define rez_menuitem_Clear                       254
#define rez_menuitem_ClearId                     254
#define rez_menuitem_ClearTitle                  254


/* *****************************************************************************
 * Alerts
 * *****************************************************************************/

#define rez_alert_About                         1001

#define rez_alert_UnknownWord                   1002

#define rez_alert_VerifyNewGame                 1003
#define rez_alert_VerifyNewGame_Cancel             1

#define rez_alert_VerifyQuitGame                1004
#define rez_alert_VerifyNewGame_Cancel             1

/* *****************************************************************************
 * Windows
 * *****************************************************************************/

/* Main Window */

#define rez_window_Wordle                       1001


/* Game Over Dialog */

#define rez_window_GameOver                     1101

#define rez_window_GameOver_ControlList         1101

#define rez_window_GameOver_NewGameButton       1101
#define rez_window_GameOver_NewGameButtonId        1
#define rez_window_GameOver_NewGameButtonTitle  1101

#define rez_window_GameOver_ShareGameButton      1102
#define rez_window_GameOver_ShareGameButtonId       2
#define rez_window_GameOver_ShareGameButtonTitle 1102

#define rez_window_GameOver_CloseButton         1103
#define rez_window_GameOver_CloseButtonId          3
#define rez_window_GameOver_CloseButtonTitle    1103


/* Help Dialog */

#define rez_window_Help                         1201

#define rez_window_Help_ControlList             1201

#define rez_window_Help_CloseButton             1202
#define rez_window_Help_CloseButtonId              1
#define rez_window_Help_CloseButtonTitle        1202

#define rez_window_Help_HelpText                1203
#define rez_window_Help_HelpTextId                 3
#define rez_window_Help_HelpTextTitle           1203

#define rez_window_Help_CorrectText             1204
#define rez_window_Help_CorrectTextId              4
#define rez_window_Help_CorrectTextTitle        1204
#define rez_window_Help_CorrectTextExample      1204

#define rez_window_Help_WrongPlaceText          1205
#define rez_window_Help_WrongPlaceTextId           5
#define rez_window_Help_WrongPlaceTextTitle     1205
#define rez_window_Help_WrongPlaceTextExample   1205

#define rez_window_Help_IncorrectText           1206
#define rez_window_Help_IncorrectTextId            6
#define rez_window_Help_IncorrectTextTitle      1206
#define rez_window_Help_IncorrectTextExample    1206

/* New Game Dialog */

#define rez_window_NewGame                      1301

#define rez_window_NewGame_ControlList          1301

#define rez_window_NewGame_OKButton             1301
#define rez_window_NewGame_OKButtonId              1
#define rez_window_NewGame_OKButtonTitle        1301

#define rez_window_NewGame_CancelButton         1302
#define rez_window_NewGame_CancelButtonId          2
#define rez_window_NewGame_CancelButtonTitle    1302

#define rez_window_NewGame_GameCodeLabel        1303
#define rez_window_NewGame_GameCodeLabelId         3
#define rez_window_NewGame_GameCodeLabelTitle   1303

#define rez_window_NewGame_GameCode             1304
#define rez_window_NewGame_GameCodeId              4

/* Share Game Dialog */

#define rez_window_ShareGame                    1401

#define rez_window_ShareGame_ControlList        1401

#define rez_window_ShareGame_CloseButton        1401
#define rez_window_ShareGame_CloseButtonId         1
#define rez_window_ShareGame_CloseButtonTitle   1401
#define rez_window_ShareGame_CloseButtonColorTable 1401

#endif /* defined(_____PROJECTNAMEASIDENTIFIER________FILEBASENAMEASIDENTIFIER_____) */
