#include "ScreenSaveLoadGame.h"

#include "ScreenMessageBox.h"
#include "BaseScreen.h"
#include "SaveLoadGame.h"
#include "GameView.h"
#include "ScriptResources.h"
#include "SaveLoadStream.h"

ScreenSaveLoadGame::ScreenSaveLoadGame(Operate opt)
{
    int TextPos[3][2] =
    {
        { 68, 28 },
        { 68, 51 },
        { 68, 74 },
    };
    memcpy(mTextPos, TextPos, sizeof(TextPos));

    mIndex = 0;
    mEmpty = "ø’µµ∞∏    ";
    mText[0] = mEmpty;
    mText[1] = mEmpty;
    mText[2] = mEmpty;
    mFileNames[0] = "./save/save0";
    mFileNames[1] = "./save/save1";
    mFileNames[2] = "./save/save2";

    mOperate = opt;
    mImgBg = (ResImage *)DatLib::getInstance()->getRes(DatLib::RES_PIC, 2, opt == ScreenSaveLoadGame::LOAD ? 16 : 15);

    std::ifstream in;
    for (int i = 0; i < 3; i++)
    {
        in.open(mFileNames[i], std::ios::in | std::ios::binary);
        if (!in.is_open())
        {
            std::cout << "opening file:" << mFileNames[i] << std::endl;
        }
        else
        {
            SaveLoadStream *saveType = new SaveLoadStream;
            in.read(saveType->mSaveData, sizeof(saveType->mSaveData));
            saveType->mLen = in.gcount();
            in.close();

            mText[i] = format(getSceneNameAndHeads(saveType, mHeadImgs[i]));

            delete saveType;
        }
    }
}

ScreenSaveLoadGame::~ScreenSaveLoadGame()
{
    delete mImgBg;

    for (int i = 0; i < 3; i++)
    {
        if (!mHeadImgs[i].empty())
        {
            for (unsigned int j = 0; j < mHeadImgs[i].size(); j++)
            {
                if (NULL != mHeadImgs[i].at(j))
                {
                    delete mHeadImgs[i].at(j);
                }
            }
        } 
    }
}

std::string ScreenSaveLoadGame::format(std::string s)
{
    while (s.length() < mEmpty.length())
    {
        s += " ";
    }
    return s;
}

std::string ScreenSaveLoadGame::getSceneNameAndHeads(SaveLoadStream *Buffer, std::vector<ResImage *> &heads)
{
    std::string name = Buffer->readString();
    int actorNum = Buffer->readInt();
    for (int i = 0; i < actorNum; i++)
    {
        heads.push_back((ResImage *)DatLib::GetRes(DatLib::RES_PIC, 1, Buffer->readInt()));
    }
    return name;

    return "";
}

void ScreenSaveLoadGame::draw(Canvas *canvas)
{
    mImgBg->draw(canvas, 1, 0, 0);

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < (int)(mHeadImgs[i].size()); j++)
        {
            ResImage *img = mHeadImgs[i][j];
            if (img != NULL)
            {
                img->draw(canvas, 7, 8 + 20 * j, mTextPos[i][1] - 6);
            }
        }
    }
    TextRender::drawText(canvas, mText[0], mTextPos[0][0], mTextPos[0][1]);
    TextRender::drawText(canvas, mText[1], mTextPos[1][0], mTextPos[1][1]);
    TextRender::drawText(canvas, mText[2], mTextPos[2][0], mTextPos[2][1]);
    TextRender::drawSelText(canvas, mText[mIndex], mTextPos[mIndex][0], mTextPos[mIndex][1]);
}

void ScreenSaveLoadGame::onKeyDown(int key)
{
    if (key == KEY_UP)
    {
        if (--mIndex < 0)
        {
            mIndex = 2;
        }
    }
    else if (key == KEY_DOWN)
    {
        if (++mIndex > 2)
        {
            mIndex = 0;
        }
    }
}

void ScreenSaveLoadGame::onKeyUp(int key)
{
    if (key == KEY_CANCEL)
    {
        GameView::getInstance()->popScreen();
    }
    else if (key == KEY_ENTER)
    {
        if (mOperate == LOAD)   // º”‘ÿ¥Êµµ
        {
            std::ifstream in;
            in.open(mFileNames[mIndex], std::ios::in | std::ios::binary);
            if (!in.is_open())
            {
                std::cout << "load file" << mFileNames[mIndex] << std::endl;
                return;
            }

            SaveLoadStream *saveType = new SaveLoadStream();
            in.read(saveType->mSaveData, sizeof(saveType->mSaveData));
            saveType->mLen = in.gcount();
            in.close();

            if (loadGame(saveType))   // ∂¡µµ≥…π¶
            {
                SaveLoadGame::startNewGame = false;
                GameView::getInstance()->changeScreen(SCREEN_MAIN_GAME);
            }
            else     // ∂¡µµ ß∞‹
            {
                SaveLoadGame::startNewGame = true;
                GameView::getInstance()->changeScreen(SCREEN_MENU);
            }

            delete saveType;
        }
        else     // ±£¥Ê¥Êµµ
        {
            if (!IsFileExist(mFileNames[mIndex].c_str()))
            {
                std::ofstream out;

                out.open(mFileNames[mIndex], std::ios::out | std::ios::binary);
                if (!out.is_open())
                {
                    std::cout << "save file:" << mFileNames[mIndex] << std::endl;
                    return;
                }

                SaveLoadStream *buffer = new SaveLoadStream;
                saveGame(buffer);

                out.write(buffer->mSaveData, buffer->mLen);
                out.close();

                delete buffer;

                GameView::getInstance()->popScreen();
                GameView::getInstance()->popScreen();
                GameView::getInstance()->popScreen();
            }
            else     // —ØŒ  «∑Ò∏≤∏«¥Êµµ
            {
                struct OnSaveOKClickListener :public ScreenMessageBox::OnOKClickListener
                {
                    std::string filename;

                    OnSaveOKClickListener(std::string filename)
                    {
                        this->filename = filename;
                    }

                    virtual void onOKClick()
                    {
                        std::ofstream out;

                        out.open(filename, std::ios::out | std::ios::binary);
                        if (!out.is_open())
                        {
                            std::cout << "save file" << filename << std::endl;
                            return;
                        }

                        SaveLoadStream *buffer = new SaveLoadStream;
                        saveGame(buffer);

                        out.write(buffer->mSaveData, buffer->mLen);
                        out.close();

                        delete buffer;

                        GameView::getInstance()->popScreen();
                        GameView::getInstance()->popScreen();
                        GameView::getInstance()->popScreen();
                    }
                };

                GameView::getInstance()->pushScreen(new ScreenMessageBox("∏≤∏«‘≠Ω¯∂»?", new OnSaveOKClickListener(mFileNames[mIndex])));
            }
        }
    }
}

bool ScreenSaveLoadGame::loadGame(SaveLoadStream *Buffer)
{
    if (!Buffer->readCheck())
    {
        return false;
    }

    if (!SaveLoadGame::read(Buffer))
    {
        return false;
    }

    if (!ScriptResources::read(Buffer))
    {
        return false;
    }

    return true;
}

bool ScreenSaveLoadGame::saveGame(SaveLoadStream *Buffer)
{
    if (!SaveLoadGame::write(Buffer))
    {
        return false;
    }

    if (!ScriptResources::write(Buffer))
    {
        return false;
    }

    if (!Buffer->writeCheck())
    {
        return false;
    }

    return true;
}

