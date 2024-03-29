#include "Popup.h"

USING_NS_CC;

using namespace ui;

namespace {
	cocos2d::Size CONFIRM_DIALOGUE_SIZE_OFFSET = Size(100, 150);
	const float ANIMATION_TIME = 0.15f;
	const float FADE_RATIO = 200;
}

namespace FONT {
	const float LABEL_OFFSET = 50;
	const float DESCRIPTION_TEXT_SIZE = 45;
	const float TITLE_TEXT_SIZE = 30;
	const char *GAME_FONT = "fonts/Marker Felt.ttf";
	const float LABEL_STROKE = 4;
}
namespace IMAGEPATH {
	const char *OK_BUTTON = "v.png";
	const char *OK_BUTTON_PRESSED = "v.png";
	const char *CANCEL_BUTTON = "x.png";
	const char *CANCEL_BUTTON_PRESSED = "x_press.png";
	const char *ADD_BUTTON = "AddKnight_button.png";
	const char *ADD_BUTTON_PRESS = "AddKnight_button_press.png";
	const char *SUB_BUTTON = "subKnight_button.png";
	const char *SUB_BUTTON_PRESS = "subKnight_button_press.png";

	const char *BACKGROUND_IMAGE_HERO = "m_PopupHero1.png";
	const char *BACKGROUND_IMAGE_HOUSE = "PopupHouse.png";
	const char *BACKGROUND_IMAGE_SHOP = "popup_shop.png";
	const char *BACKGROUND_IMAGE_TOWNHALL = "popup_create_knight.png";
	const char *TOWN_HALL_BUTTON = "HallTown_button.png";
	const char *TOWN_HALL_BUTTON_PRESS = "HallTown_button_press.png";
	const char *SCOUT_TOWN_BUTTON = "ScoutTown2D_button.png";
	const char *SCOUT_TOWN_BUTTON_PRESS = "ScoutTown2D_button_press.png";
	const char *HOUSE_MAIN_BUTTON = "HouseMain_button.png";
	const char *HOUSE_MAIN_BUTTON_PRESS = "HouseMain_button_press.png";
	const char *HOUSE_STORE_BUTTON = "StoreHouse_button.png";
	const char *HOUSE_STORE_BUTTON_PRESS = "StoreHouse_button_press.png";
	const char *HOUSE_DECORATE_BUTTON = "HouseDecorate_button.png";
	const char *HOUSE_DECORATE_BUTTON_PRESS = "HouseDecorate_button_press";
	const char *GOLD_MINE_BUTTON = "gold_mine_button.png";
	const char *GOLD_MINE_BUTTON_PRESS = "gold_mine_button_press.png";
	const char *AXE_ITEM = "axe.png";
	const char *KNIHT_ITEM = "itemKnight.png";
	const char *BACKGROUND_POPUP_CHOOSE_KNIGHT = "Popup_choose_knight.png";
}
namespace UICustom {

	PopupDelegates *PopupDelegates::create()
	{
		PopupDelegates *node = new (std::nothrow)PopupDelegates();
		if (node && node->init())
		{
			node->autorelease();
			return node;
		}
		CC_SAFE_DELETE(node);
		return nullptr;
	}

	bool PopupDelegates::init()
	{
		Size winSize = Director::getInstance()->getWinSize();

		if (!LayerRadialGradient::initWithColor(Color4B(0, 0, 0, 0), Color4B(0, 0, 0, FADE_RATIO), winSize.width / 1.7f, winSize / 2, 0.075f))
		{
			return false;
		}
		this->setOpacity(0);
		show(true);
		this->setUpTouches();


		return true;
	}

	void PopupDelegates::show(const bool animated)
	{
		if (animated) {
			this->runAction(FadeTo::create(ANIMATION_TIME, FADE_RATIO));
		}
		else {
			this->setOpacity(FADE_RATIO);
		}
	}

	void PopupDelegates::dismiss(const bool animated)
	{
		if (animated) {
			this->runAction(Sequence::create(FadeTo::create(ANIMATION_TIME, 0), RemoveSelf::create(), NULL));
		}
		else {
			this->removeFromParentAndCleanup(true);
		}
	}

	void PopupDelegates::setUpTouches()
	{

		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = [=](Touch* touch, Event* event) {
			if (_bg) {
				if (!_bg->getBoundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation())))
				{
					this->dismiss(true);
				}
			}
			else {
				this->dismiss(true);
			}
			return true;
		};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	}

	// popup

	Popup *Popup::createAsMessage(const std::string &title, const std::string &msg)
	{
		return createAsConfirmDialogue(title, msg, NULL);
	}

	Popup *Popup::createAsConfirmDialogue(const std::string &title, const std::string &msg, const std::function<void()> &YesFunc)
	{
		return create(title, msg, NULL, YesFunc);
	}

	Popup *Popup::create(const std::string &title, const std::string &msg, cocos2d::Label *lbl, const std::function<void()> &YesFunc)
	{
		Popup *node = new (std::nothrow)Popup();
		Size winSize = Director::getInstance()->getWinSize();
		if (node && node->init())
		{
			if (!lbl) {
				lbl = Label::createWithTTF(msg, FONT::GAME_FONT, FONT::DESCRIPTION_TEXT_SIZE);
			}
			lbl->setPosition(winSize.width / 2, winSize.height / 2 - FONT::LABEL_OFFSET / 2);
			lbl->enableOutline(Color4B::BLACK, FONT::LABEL_STROKE);
			lbl->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
			lbl->enableShadow(Color4B::BLACK, Size(0, -2));

			if (YesFunc) {
				MenuItemImage *yesButton = MenuItemImage::create(IMAGEPATH::OK_BUTTON, IMAGEPATH::OK_BUTTON_PRESSED, [=](Ref *sender) {

					YesFunc();
					node->dismiss(true);
				});

				MenuItemImage *noButton = MenuItemImage::create(IMAGEPATH::CANCEL_BUTTON, IMAGEPATH::CANCEL_BUTTON_PRESSED, [node](Ref *sender) {
					node->dismiss(true);
				});

				Menu *menu = Menu::create(noButton, NULL);
				node->addChild(menu, 2);
				menu->setPosition(winSize.width / 2, winSize.height / 2 - lbl->getContentSize().height / 2 - 75);
				menu->alignItemsHorizontallyWithPadding(FONT::LABEL_OFFSET / 2);

				lbl->setPosition(winSize / 2);
				CONFIRM_DIALOGUE_SIZE_OFFSET = Size(CONFIRM_DIALOGUE_SIZE_OFFSET.width, 300);
			}
			node->addChild(lbl, 10);
			node->initBg(lbl->getContentSize() + CONFIRM_DIALOGUE_SIZE_OFFSET, title);
			node->autorelease();
			return node;
		}

		CC_SAFE_DELETE(node);
		return nullptr;
	}


	void Popup::initBg(Size size, const std::string &title)
	{

		Size winSize = Director::getInstance()->getWinSize();

		_bg = ui::ImageView::create(IMAGEPATH::BACKGROUND_IMAGE_HERO);
		this->addChild(_bg);

		_bg->setPosition(Point(winSize.width / 2, winSize.height / 2));
		_bg->setScale9Enabled(true);
		//_bg->setContentSize(size);

		ui::ImageView *fill = ui::ImageView::create(IMAGEPATH::BACKGROUND_IMAGE_HERO);
		//_bg->addChild(fill);
		fill->setColor(Color3B(210, 210, 210));
		fill->setScale9Enabled(true);
		fill->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		fill->setPosition(Point(FONT::LABEL_OFFSET / 4, FONT::LABEL_OFFSET / 4));
		//fill->setContentSize(Size(size.width - FONT::LABEL_OFFSET / 2, size.height - FONT::LABEL_OFFSET * 2));


		Label *heading = Label::createWithTTF(title, FONT::GAME_FONT, FONT::TITLE_TEXT_SIZE);
		heading->setPosition(_bg->getContentSize().width / 2, _bg->getContentSize().height - FONT::LABEL_OFFSET / 2.3);
		_bg->addChild(heading);
		heading->enableOutline(Color4B::BLACK, FONT::LABEL_STROKE);
		heading->enableShadow(Color4B::BLACK, Size(0, -3));
	}
	//-------------------------------------------------------------
	//---------------POPUP CONTAIN HOUSE---------------------------
	//-------------------------------------------------------------

	PopupHouse * PopupHouse::createAsMessage(const std::string & title, const std::string & msg)
	{
		//return createAsConfirmDialogue(title, msg, nullptr);
		return nullptr;
	}
	PopupHouse * PopupHouse::createAsConfirmDialogue(const std::string & title, 
		const std::string & msg, const std::function<void()>& townHall, 
		const std::function<void()>& scoutTown, const std::function<void()>& mainHouse, 
		const std::function<void()>& storeHouse, const std::function<void()>& DecorateHouse, 
		const std::function<void()>& YesFunc5, const std::function<void()>& YesFunc6)
	{
		return create(title, msg, NULL, townHall, scoutTown, mainHouse, storeHouse, DecorateHouse, YesFunc5, YesFunc6);
	}

	PopupHouse * PopupHouse::create(const std::string & title, const std::string & msg, 
		cocos2d::Label * lbl, const std::function<void()>& mainHouse,
		const std::function<void()>& townHall, const std::function<void()>& storeHouse,
		const std::function<void()>& scoutTown, const std::function<void()>& DecorateHouse,
		const std::function<void()>& YesFunc5, const std::function<void()>& YesFunc6)
	{
		PopupHouse *node = new (std::nothrow)PopupHouse();
		Size winSize = Director::getInstance()->getWinSize();
		if (node && node->init())
		{
			if (!lbl) {
				lbl = Label::createWithTTF(msg, FONT::GAME_FONT, FONT::DESCRIPTION_TEXT_SIZE);
			}
			lbl->setPosition(winSize.width / 2, winSize.height / 2 - FONT::LABEL_OFFSET / 2);
			lbl->enableOutline(Color4B::BLACK, FONT::LABEL_STROKE);
			lbl->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
			lbl->enableShadow(Color4B::BLACK, Size(0, -2));

			if (mainHouse) {
				MenuItemImage *houseMain = MenuItemImage::create(IMAGEPATH::HOUSE_MAIN_BUTTON, IMAGEPATH::HOUSE_MAIN_BUTTON_PRESS, [=](Ref *sender) {
					mainHouse();
					node->dismiss(true);
				});
				MenuItemImage *houseDecorate = MenuItemImage::create(IMAGEPATH::GOLD_MINE_BUTTON, IMAGEPATH::GOLD_MINE_BUTTON_PRESS, [=](Ref *sender) {
					DecorateHouse();
					node->dismiss(true);
				});
				MenuItemImage *StoreHouse = MenuItemImage::create(IMAGEPATH::HOUSE_STORE_BUTTON, IMAGEPATH::HOUSE_STORE_BUTTON_PRESS, [=](Ref *sender) {
					storeHouse();
					node->dismiss(true);
				});
				MenuItemImage *hallTownImage = MenuItemImage::create(IMAGEPATH::TOWN_HALL_BUTTON, IMAGEPATH::TOWN_HALL_BUTTON_PRESS, [=](Ref *sender) {
					townHall();
					node->dismiss(true);
				});
				MenuItemImage *scoutTownImage = MenuItemImage::create(IMAGEPATH::SCOUT_TOWN_BUTTON, IMAGEPATH::SCOUT_TOWN_BUTTON_PRESS, [=](Ref *sender) {
					scoutTown();
					node->dismiss(true);
				});
				MenuItemImage *noButton = MenuItemImage::create(IMAGEPATH::CANCEL_BUTTON, IMAGEPATH::CANCEL_BUTTON_PRESSED, [node](Ref *sender) {
					node->dismiss(true);
				});

				Menu *menu = Menu::create(houseMain, hallTownImage, StoreHouse, scoutTownImage, houseDecorate,NULL);
				node->addChild(menu, 2);
				menu->setPosition(winSize.width / 2, winSize.height / 1.5 - lbl->getContentSize().height / 2 - 75);
				menu->alignItemsHorizontallyWithPadding(FONT::LABEL_OFFSET / 1.9);

				// Menu label
				Label *mainHouseLabel = Label::createWithTTF("Hall Town", "fonts/Marker Felt.ttf", 20);
				Label *hallTownLabel = Label::createWithTTF("Barrack", "fonts/Marker Felt.ttf", 20);
				Label *storeHouseLabel = Label::createWithTTF("      Shop", "fonts/Marker Felt.ttf", 20);
				Label *scoutTownLabel = Label::createWithTTF("Scout Town", "fonts/Marker Felt.ttf", 20);
				Label *decorateHouseLabel	 = Label::createWithTTF("Gold mine", "fonts/Marker Felt.ttf", 20);

				MenuItemLabel *mainHouseLabelItem = MenuItemLabel::create(mainHouseLabel);
				MenuItemLabel *hallTownLabelItem = MenuItemLabel::create(hallTownLabel);
				MenuItemLabel *storeHouseLabelItem = MenuItemLabel::create(storeHouseLabel);
				MenuItemLabel *scoutTownLabelItem = MenuItemLabel::create(scoutTownLabel);
				MenuItemLabel *decorateHouseLabelItem = MenuItemLabel::create(decorateHouseLabel);

				auto menuLabel = Menu::create(mainHouseLabelItem, hallTownLabelItem,
					storeHouseLabelItem, scoutTownLabelItem, decorateHouseLabelItem, NULL);
				menuLabel->setPosition(winSize.width / 1.97, winSize.height / 2.2);
				node->addChild(menuLabel, 2);
				menuLabel->alignItemsHorizontallyWithPadding(FONT::LABEL_OFFSET * 1.2);

				// label gold
				auto *mainHouseLabelCoin = Label::createWithTTF("5000", "fonts/arial.ttf", 18);
				mainHouseLabelCoin->setColor(Color3B::YELLOW);
				auto *hallTownLabelCoin = Label::createWithTTF("2000", "fonts/arial.ttf", 18);
				hallTownLabelCoin->setColor(Color3B::YELLOW);
				auto *storeHouseLabelCoin = Label::createWithTTF("1500", "fonts/arial.ttf", 18);
				storeHouseLabelCoin->setColor(Color3B::YELLOW);
				auto *scoutTownLabelCoin = Label::createWithTTF("500", "fonts/arial.ttf", 18);
				scoutTownLabelCoin->setColor(Color3B::YELLOW);
				auto *decorateHouseLabelCoin = Label::createWithTTF("1000", "fonts/arial.ttf", 18);
				decorateHouseLabelCoin->setColor(Color3B::YELLOW);


				MenuItemLabel *mainHouseLabelItemCoin = MenuItemLabel::create(mainHouseLabelCoin);
				MenuItemLabel *hallTownLabelItemCoin = MenuItemLabel::create(hallTownLabelCoin);
				MenuItemLabel *storeHouseLabelItemCoin = MenuItemLabel::create(storeHouseLabelCoin);
				MenuItemLabel *scoutTownLabelItemCoin = MenuItemLabel::create(scoutTownLabelCoin);
				MenuItemLabel *decorateHouseLabelItemCoin = MenuItemLabel::create(decorateHouseLabelCoin);

				auto menuLabelCoin = Menu::create(mainHouseLabelItemCoin, hallTownLabelItemCoin,
					storeHouseLabelItemCoin, scoutTownLabelItemCoin, decorateHouseLabelItemCoin, NULL);
				menuLabelCoin->setPosition(winSize.width / 1.95, winSize.height / 2.6);
				node->addChild(menuLabelCoin, 2);
				menuLabelCoin->alignItemsHorizontallyWithPadding(FONT::LABEL_OFFSET * 1.8);

				// sprite gold
				auto coin1 = Sprite::create("Item/coin.png");
				auto coin2 = Sprite::create("Item/coin.png");
				auto coin3 = Sprite::create("Item/coin.png");
				auto coin4 = Sprite::create("Item/coin.png");
				auto coin5 = Sprite::create("Item/coin.png");
			
				auto coinMenuSprite1 = MenuItemSprite::create(coin1, NULL);
				auto coinMenuSprite2 = MenuItemSprite::create(coin2, NULL);
				auto coinMenuSprite3 = MenuItemSprite::create(coin3, NULL);
				auto coinMenuSprite4 = MenuItemSprite::create(coin4, NULL);
				auto coinMenuSprite5 = MenuItemSprite::create(coin5, NULL);

				auto menuSprite = Menu::create(coinMenuSprite1, coinMenuSprite2, coinMenuSprite3, 
					coinMenuSprite4, coinMenuSprite5, NULL);
				node->addChild(menuSprite, 2);
				menuSprite->setPosition(winSize.width / 2.15, winSize.height / 2.6);
				menuSprite->alignItemsHorizontallyWithPadding(FONT::LABEL_OFFSET * 1.35);
				lbl->setPosition(winSize / 2);
				CONFIRM_DIALOGUE_SIZE_OFFSET = Size(CONFIRM_DIALOGUE_SIZE_OFFSET.width, 300);
			}
			node->addChild(lbl, 10);
			node->initBg(lbl->getContentSize() + CONFIRM_DIALOGUE_SIZE_OFFSET, title);
			node->autorelease();
			return node;
		}

		CC_SAFE_DELETE(node);
		return nullptr;
	}

	void PopupHouse::initBg(Size size, const std::string &title)
	{

		Size winSize = Director::getInstance()->getWinSize();

		_bg = ui::ImageView::create(IMAGEPATH::BACKGROUND_IMAGE_HOUSE);
		this->addChild(_bg);

		_bg->setPosition(Point(winSize.width / 2, winSize.height / 2));
		//_bg->setScale9Enabled(true);
		//_bg->setContentSize(size);

		ui::ImageView *fill = ui::ImageView::create(IMAGEPATH::BACKGROUND_IMAGE_HOUSE);
		//_bg->addChild(fill);
		fill->setColor(Color3B(210, 210, 210));
		fill->setScale9Enabled(true);
		fill->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		fill->setPosition(Point(FONT::LABEL_OFFSET / 4, FONT::LABEL_OFFSET / 4));
		//fill->setContentSize(Size(size.width - FONT::LABEL_OFFSET / 2, size.height - FONT::LABEL_OFFSET * 2));

		Label *heading = Label::createWithTTF(title, FONT::GAME_FONT, FONT::TITLE_TEXT_SIZE);
		heading->setPosition(_bg->getContentSize().width / 2, _bg->getContentSize().height / 1.5 - FONT::LABEL_OFFSET / 2.3);
		_bg->addChild(heading);
		heading->enableOutline(Color4B::BLACK, FONT::LABEL_STROKE);
		heading->enableShadow(Color4B::BLACK, Size(0, -3));
	}

	// popup hero
	PopupHero * PopupHero::createAsMessage(const std::string & title, const std::string & msg)
	{
		return nullptr;
	}
	PopupHero * PopupHero::createAsConfirmDialogue(const std::string & title, const std::string & msg, 
		std::vector<Item *> &menuItem, const std::function<void()>& YesFunc , MenuItemLabel * _menuLabelHp
		, MenuItemLabel * _menuLabelDamage, MenuItemLabel * _menuLabelArmor, MenuItemLabel * _menuLabelStrength)
	{
		return create(title, msg, NULL, menuItem, YesFunc, _menuLabelHp, _menuLabelDamage, _menuLabelArmor, _menuLabelStrength);
	}
	PopupHero * PopupHero::create(const std::string & title, const std::string & msg, cocos2d::Label * lbl, 
		std::vector<Item *> &menuItem, const std::function<void()>& YesFunc , MenuItemLabel * _menuLabelHp
	, MenuItemLabel * _menuLabelDamage, MenuItemLabel * _menuLabelArmor, MenuItemLabel * _menuLabelStrength)
	{
		PopupHero *node = new (std::nothrow)PopupHero();
		Size winSize = Director::getInstance()->getWinSize();
		if (node && node->init())
		{
			if (!lbl) {
				lbl = Label::createWithTTF(msg, FONT::GAME_FONT, FONT::DESCRIPTION_TEXT_SIZE);
			}
			lbl->setPosition(winSize.width / 2, winSize.height / 2 - FONT::LABEL_OFFSET / 2);
			lbl->enableOutline(Color4B::BLACK, FONT::LABEL_STROKE);
			lbl->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
			lbl->enableShadow(Color4B::BLACK, Size(0, -2));

			// Atribute Label 
			auto labelHp = Label::createWithTTF("Hp :", "fonts/arial.ttf", 15);
			auto labelDamage = Label::createWithTTF("Damage :", "fonts/arial.ttf", 15);
			auto labelArmor = Label::createWithTTF("Armor :", "fonts/arial.ttf", 15);
			auto labelStrength = Label::createWithTTF("Strength :", "fonts/arial.ttf", 15);
			labelStrength->setVisible(false);

			auto itemLabelHp = MenuItemLabel::create(labelHp);
			auto itemLabelDamage = MenuItemLabel::create(labelDamage);
			auto itemLabelArmor = MenuItemLabel::create(labelArmor);
			auto itemLabelStrength = MenuItemLabel::create(labelStrength);

			Menu *menuLabelAtributeHero = Menu::create(itemLabelHp, itemLabelDamage,
				itemLabelArmor, itemLabelStrength, NULL);
			node->addChild(menuLabelAtributeHero, 1);
			menuLabelAtributeHero->setPosition(winSize.width * 0.58, winSize.height * 0.16);
			menuLabelAtributeHero->alignItemsVerticallyWithPadding(5);

			// Atribute hero
			_menuLabelStrength->setVisible(false);
			auto menuLabelAtributeOfHero = Menu::create(_menuLabelHp,
				_menuLabelDamage, _menuLabelArmor, _menuLabelStrength, NULL);
			node->addChild(menuLabelAtributeOfHero, 1);
			menuLabelAtributeOfHero->setPosition(winSize.width * 0.62, winSize.height * 0.16);
			menuLabelAtributeOfHero->alignItemsVerticallyWithPadding(5);

			// Add atribute of hero 


			//
			int heightBag = 6;
			int widthBag = 4;
			int id = 0;
			int ID = 9;

			float posWidth = winSize.width / 2.85;
			float posHeight = winSize.height / 1.36;
				// remove when out of popup
				for (int i = 0; i < menuItem.size(); i++)
				{
					if (menuItem.at(i)->checkAddchild == true)
					{
						menuItem.at(i)->getButton()->removeFromParent();
						menuItem.at(i)->checkAddchild = false;
					}
					if (menuItem.at(i)->checkAddchild == false)
					{
						node->addChild(menuItem.at(i)->getButton(), 2);
						menuItem.at(i)->checkAddchild = true;
					}
				}

				for (int i = 0; i < menuItem.size(); i++)
				{
					if (menuItem.at(i)->GetState() == ID_STATE_HOME)
					{

						menuItem.at(i)->getButton()->setPosition(Vec2(posWidth, posHeight));
						posWidth += menuItem.at(i)->getButton()->getContentSize().width * 0.85;
						menuItem.at(i)->setPrePosition(menuItem.at(i)->getButton()->getPosition());
						if ((i + 1)% 4 == 0 && i > 0)
						{
							posWidth = winSize.width / 2.85;
							posHeight -= menuItem.at(i)->getButton()->getContentSize().height * 0.9;
						}
					}

				}

				//std::string add = "+ (";
				//std::string hp = 
				//auto labelHp = Label::createWithTTF(add  , "fonts/arial.ttf", 15);
				//auto labelDamage = Label::createWithTTF(add, "fonts/arial.ttf", 15);
				//auto labelArmor = Label::createWithTTF(add, "fonts/arial.ttf", 15);
				//auto labelStrength = Label::createWithTTF(add, "fonts/arial.ttf", 15);


				//auto itemLabelHp = MenuItemLabel::create(labelHp);
				//auto itemLabelDamage = MenuItemLabel::create(labelDamage);
				//auto itemLabelArmor = MenuItemLabel::create(labelArmor);
				//auto itemLabelStrength = MenuItemLabel::create(labelStrength);

				//Menu *menuLabelAtributeHero = Menu::create(itemLabelHp, itemLabelDamage,
				//	itemLabelArmor, itemLabelStrength, NULL);
				//node->addChild(menuLabelAtributeHero, 1);
				//menuLabelAtributeHero->setPosition(winSize.width * 0.58, winSize.height * 0.16);
				//menuLabelAtributeHero->alignItemsVerticallyWithPadding(5);
				
				lbl->setPosition(winSize / 2);
				CONFIRM_DIALOGUE_SIZE_OFFSET = Size(CONFIRM_DIALOGUE_SIZE_OFFSET.width, 300);

			node->addChild(lbl, 10);
			node->initBg(lbl->getContentSize() + CONFIRM_DIALOGUE_SIZE_OFFSET, title);



			node->autorelease();
			return node;
		}

		CC_SAFE_DELETE(node);
		return nullptr;
	}
	void PopupHero::initBg(const cocos2d::Size size, const std::string & title)
	{
		Size winSize = Director::getInstance()->getWinSize();

		_bg = ui::ImageView::create(IMAGEPATH::BACKGROUND_IMAGE_HERO);
		this->addChild(_bg);

		_bg->setPosition(Point(winSize.width / 2, winSize.height / 2));
		_bg->setScale9Enabled(true);
		//_bg->setContentSize(size);
		//_bg->setVisible(false);

		ui::ImageView *fill = ui::ImageView::create(IMAGEPATH::BACKGROUND_IMAGE_HERO);
		//_bg->addChild(fill);
		fill->setColor(Color3B(210, 210, 210));
		fill->setScale9Enabled(true);
		fill->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		fill->setPosition(Point(FONT::LABEL_OFFSET / 4, FONT::LABEL_OFFSET / 4));
		//fill->setContentSize(Size(size.width - FONT::LABEL_OFFSET / 2, size.height - FONT::LABEL_OFFSET * 2));


		Label *heading = Label::createWithTTF(title, FONT::GAME_FONT, FONT::TITLE_TEXT_SIZE);
		heading->setPosition(_bg->getContentSize().width / 2, _bg->getContentSize().height - FONT::LABEL_OFFSET / 2.3);
		_bg->addChild(heading);
		heading->enableOutline(Color4B::BLACK, FONT::LABEL_STROKE);
		heading->enableShadow(Color4B::BLACK, Size(0, -3));
	}
	//-------------------------------------------------------------------------------
	// -------------------POPUP CREATE KNIGHT OF TOWN HALL---------------------------
	//-------------------------------------------------------------------------------
	PopupTownHall * PopupTownHall::createAsMessage(const std::string & title, const std::string & msg)
	{
		return nullptr;
	}
	PopupTownHall * PopupTownHall::createAsConfirmDialogue(const std::string & title, const std::string & msg, const std::function<void()>& YesFunc)
	{
		return create(title, msg , NULL, YesFunc);
	}
	PopupTownHall * PopupTownHall::create(const std::string & title, const std::string & msg, cocos2d::Label * lbl, const std::function<void()>& YesFunc)
	{
		PopupTownHall *node = new (std::nothrow)PopupTownHall();
		Size winSize = Director::getInstance()->getWinSize();
		if (node && node->init())
		{
			if (!lbl) {
				lbl = Label::createWithTTF(msg, FONT::GAME_FONT, FONT::DESCRIPTION_TEXT_SIZE);
			}
			lbl->setPosition(winSize.width / 2, winSize.height / 2 - FONT::LABEL_OFFSET / 2);
			lbl->enableOutline(Color4B::BLACK, FONT::LABEL_STROKE);
			lbl->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
			lbl->enableShadow(Color4B::BLACK, Size(0, -2));

			if (YesFunc) {
				MenuItemImage *knight = MenuItemImage::create(IMAGEPATH::KNIHT_ITEM, IMAGEPATH::KNIHT_ITEM, [=](Ref *sender) {
					YesFunc();
					node->dismiss(true);
				});
				knight->setScale(0.7);
				auto labelCoin = Label::createWithTTF("1000", "fonts/arial.ttf", 20);
				labelCoin->setColor(Color3B::YELLOW);
				auto menuLabel = MenuItemLabel::create(labelCoin);
				auto spriteCoin = Sprite::create("item/coin.png");
				auto menuCoin = MenuItemSprite::create(spriteCoin, NULL);

				Menu *menu = Menu::create(knight, NULL);
				node->addChild(menu, 2);
				menu->setPosition(winSize.width / 2.27, winSize.height / 2.1);
				Menu *menuGold = Menu::create(menuCoin, menuLabel, NULL);
				menuGold->setPosition(winSize.width / 1.8, winSize.height / 2.1);

				node->addChild(menuGold, 2);
				menu->alignItemsHorizontallyWithPadding(FONT::LABEL_OFFSET / 2);
				menuGold->alignItemsHorizontallyWithPadding(FONT::LABEL_OFFSET / 5);


				lbl->setPosition(winSize / 2);
				CONFIRM_DIALOGUE_SIZE_OFFSET = Size(CONFIRM_DIALOGUE_SIZE_OFFSET.width, 300);
			}
			node->addChild(lbl, 10);
			node->initBg(lbl->getContentSize() + CONFIRM_DIALOGUE_SIZE_OFFSET, title);
			node->autorelease();
			return node;
		}

		CC_SAFE_DELETE(node);
		return nullptr;
	}
	void PopupTownHall::initBg(const cocos2d::Size size, const std::string & title)
	{
		Size winSize = Director::getInstance()->getWinSize();

		_bg = ui::ImageView::create(IMAGEPATH::BACKGROUND_IMAGE_TOWNHALL);
		this->addChild(_bg);

		_bg->setPosition(Point(winSize.width / 2, winSize.height / 2));
		_bg->setScale9Enabled(true);
		//_bg->setContentSize(size);

		ui::ImageView *fill = ui::ImageView::create(IMAGEPATH::BACKGROUND_IMAGE_TOWNHALL);
		//_bg->addChild(fill);
		fill->setColor(Color3B(210, 210, 210));
		fill->setScale9Enabled(true);
		fill->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		fill->setPosition(Point(FONT::LABEL_OFFSET / 4, FONT::LABEL_OFFSET / 4));
		//fill->setContentSize(Size(size.width - FONT::LABEL_OFFSET / 2, size.height - FONT::LABEL_OFFSET * 2));

		Label *heading = Label::createWithTTF(title, FONT::GAME_FONT, FONT::TITLE_TEXT_SIZE);
		heading->setPosition(_bg->getContentSize().width / 2, _bg->getContentSize().height - FONT::LABEL_OFFSET / 2.3);
		_bg->addChild(heading);
		heading->enableOutline(Color4B::BLACK, FONT::LABEL_STROKE);
		heading->enableShadow(Color4B::BLACK, Size(0, -3));
	}
	//-------------------------------------------------------------------------------
	// -------------------POPUP CREATE KNIGHT OF TOWN HALL---------------------------
	//-------------------------------------------------------------------------------
	PopupChooseKnight * PopupChooseKnight::createAsMessage(const std::string & title, const std::string & msg)
	{
		return nullptr;
	}
	PopupChooseKnight * PopupChooseKnight::createAsConfirmDialogue(const std::string & title, const std::string & msg, 
		const std::function<void()>& add, const std::function<void()>& sub,
		const std::function<void()>& knight)
	{
		return create(title, msg, NULL, NULL, NULL, NULL, add, sub, knight);
	}
	PopupChooseKnight * PopupChooseKnight::create(const std::string & title, const std::string & msg, 
		cocos2d::Label * intSum, cocos2d::Label * labelSum, cocos2d::Label * labelChoose, 
		cocos2d::Label * intChoose, const std::function<void()>& add, const std::function<void()>& sub, 
		const std::function<void()>& knight)
	{
		PopupChooseKnight *node = new (std::nothrow)PopupChooseKnight();
		Size winSize = Director::getInstance()->getWinSize();
		if (node && node->init())
		{
			if (!intSum) {
				intSum = Label::createWithTTF(msg, FONT::GAME_FONT, FONT::DESCRIPTION_TEXT_SIZE);
			}
			intSum->setSystemFontSize(24);
			intSum->setPosition(winSize.width / 1.9, winSize.height / 1.95 - FONT::LABEL_OFFSET / 2);
			intSum->enableOutline(Color4B::BLACK, FONT::LABEL_STROKE);
			intSum->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
			intSum->enableShadow(Color4B::BLACK, Size(0, -2));

			labelSum->setSystemFontSize(24);
			labelSum->setPosition(winSize.width / 2.1, winSize.height / 1.95 - FONT::LABEL_OFFSET / 2);
			labelSum->enableOutline(Color4B::BLACK, FONT::LABEL_STROKE);
			labelSum->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
			labelSum->enableShadow(Color4B::BLACK, Size(0, -2));

			intChoose->setSystemFontSize(24);
			intChoose->setPosition(winSize.width / 1.65, winSize.height / 1.7 - FONT::LABEL_OFFSET / 2);
			intChoose->enableOutline(Color4B::BLACK, FONT::LABEL_STROKE);
			intChoose->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
			intChoose->enableShadow(Color4B::BLACK, Size(0, -2));

			labelChoose->setSystemFontSize(24);
			labelChoose->setPosition(winSize.width / 1.8, winSize.height / 1.7 - FONT::LABEL_OFFSET / 2);
			labelChoose->enableOutline(Color4B::BLACK, FONT::LABEL_STROKE);
			labelChoose->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
			labelChoose->enableShadow(Color4B::BLACK, Size(0, -2));
			if (add) {
				// menu knight
				//MenuItemImage *knightItem = MenuItemImage::create(IMAGEPATH::KNIHT_ITEM, IMAGEPATH::KNIHT_ITEM, [=](Ref *sender) {
				//	knight();
				//	node->dismiss(true);
				//});
				//knightItem->setScale(0.5);
				//auto menuKnight = Menu::create(knightItem, NULL);
				//node->addChild(menuKnight, 2);
				//menuKnight->setPosition(winSize.width / 2, winSize.height / 1.9);
				//Add textfile

				//Add menu add sub
				auto addButton = MenuItemImage::create(IMAGEPATH::ADD_BUTTON, IMAGEPATH::ADD_BUTTON_PRESS, [=](Ref *sender) {
					add();
				});
				auto subButton = MenuItemImage::create(IMAGEPATH::SUB_BUTTON, IMAGEPATH::SUB_BUTTON_PRESS, [=](Ref *sender) {
					sub();
				});
				Menu *menuAddSub = Menu::create(subButton, addButton, NULL);
				node->addChild(menuAddSub, 2);
				//menuAddSub->setPosition(winSize.width * 0.726, winSize.height * 0.483);
				menuAddSub->setPosition(intChoose->getContentSize().width * 5 + intChoose->getPosition().x, intChoose->getPosition().y);
				menuAddSub->alignItemsHorizontallyWithPadding(subButton->getContentSize().width / 1.1);
				CONFIRM_DIALOGUE_SIZE_OFFSET = Size(CONFIRM_DIALOGUE_SIZE_OFFSET.width, 300);
			}
			node->addChild(intSum, 10);
			node->addChild(intChoose, 10);
			node->addChild(labelSum, 10);
			node->addChild(labelChoose, 10);
			node->initBg(intSum->getContentSize() + CONFIRM_DIALOGUE_SIZE_OFFSET, title);
			node->autorelease();
			return node;
		}
		CC_SAFE_DELETE(node);
		return nullptr;
	}
	void PopupChooseKnight::initBg(const cocos2d::Size size, const std::string & title)
	{
		Size winSize = Director::getInstance()->getWinSize();

		_bg = ui::ImageView::create(IMAGEPATH::BACKGROUND_POPUP_CHOOSE_KNIGHT);
		this->addChild(_bg);

		_bg->setPosition(Point(winSize * 0.6));
		_bg->setScale9Enabled(true);
		//_bg->setContentSize(Size(_bg->getContentSize().width * 1.5, _bg->getContentSize().height));
		_bg->setScale(1.5);

		//ui::ImageView *fill = ui::ImageView::create(IMAGEPATH::BACKGROUND_POPUP_CHOOSE_KNIGHT);
		//_bg->addChild(fill);
		//fill->setColor(Color3B(210, 210, 210));
		//fill->setScale9Enabled(true);
		//fill->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		//fill->setPosition(Point(FONT::LABEL_OFFSET / 4, FONT::LABEL_OFFSET / 4));
		//fill->setContentSize(Size(size.width - FONT::LABEL_OFFSET / 2, size.height - FONT::LABEL_OFFSET * 2));

		Label *heading = Label::createWithTTF(title, FONT::GAME_FONT, FONT::TITLE_TEXT_SIZE);
		heading->setPosition(_bg->getContentSize().width / 2, _bg->getContentSize().height - FONT::LABEL_OFFSET / 2.3);
		_bg->addChild(heading);
		heading->enableOutline(Color4B::BLACK, FONT::LABEL_STROKE);
		heading->enableShadow(Color4B::BLACK, Size(0, -3));
	}

	// Popup for shop
	PopupShop * PopupShop::createAsMessage(const std::string & title, const std::string & msg)
	{
		return nullptr;
	}
	PopupShop * PopupShop::createAsConfirmDialogue(int &m_gold, const std::string & title, const std::string & msg, std::vector<Item*>& menuItem,
		std::vector<Item *> &menuItemShop, const std::function<void()>& YesFunc)
	{
		return create(m_gold, title, msg, NULL, menuItem, menuItemShop, YesFunc);
	}
	PopupShop * PopupShop::create(int &_gold, const std::string & title, const std::string & msg, cocos2d::Label * lbl, std::vector<Item*>& menuItem,
		std::vector<Item *>& menuItemShop, const std::function<void()>& YesFunc)
	{
		PopupShop *node = new (std::nothrow)PopupShop();
		Size winSize = Director::getInstance()->getWinSize();
		if (node && node->init())
		{
			if (!lbl) {
				lbl = Label::createWithTTF(msg, FONT::GAME_FONT, FONT::DESCRIPTION_TEXT_SIZE);
			}
			lbl->setPosition(winSize.width / 2, winSize.height / 2 - FONT::LABEL_OFFSET / 2);
			lbl->enableOutline(Color4B::BLACK, FONT::LABEL_STROKE);
			lbl->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
			lbl->enableShadow(Color4B::BLACK, Size(0, -2));

			int heightBag = 6;
			int widthBag = 4;
			int id = 0;
			int ID = 9;

			float posWidthShop = winSize.width / 2.3;
			float posHeightShop = winSize.height / 1.62;

			if (YesFunc) {
				//auto potionBlue = ui::Button::


				for (int i = 0; i < menuItemShop.size(); i++)
				{
					if (menuItemShop.at(i)->checkAddchild == true)
					{
						menuItemShop.at(i)->getButton()->removeFromParent();
						menuItemShop.at(i)->checkAddchild = false;
					}
					if (menuItemShop.at(i)->checkAddchild == false)
					{
						node->addChild(menuItemShop.at(i)->getButton(), 2);
						menuItemShop.at(i)->checkAddchild = true;
					}
				}

				// menu shop

				for (int i = 0; i < menuItemShop.size(); i++)
				{
					if (menuItemShop.at(i)->GetState() == ID_STATE_SHOP)
					{
						menuItemShop.at(i)->getButton()->removeAllComponents();
						
						menuItemShop.at(i)->getButton()->addTouchEventListener([&](Ref *sender, ui::Widget::TouchEventType type)
						{
							auto button = dynamic_cast<Button *>(sender);
							switch (type)
							{
							case cocos2d::ui::Widget::TouchEventType::BEGAN:
							{
								for (int i = 0; i < menuItemShop.size(); i++)
								{
									if (button->getTag() == menuItemShop.at(i)->getButton()->getTag() && menuItemShop.at(i)->getGoldItem() <= _gold)
									{
										
										auto item = new Item(menuItemShop.at(i)->GetId(), ID_STATE_HOME);
										_gold -= item->getGoldItem();
										item->getButton()->retain();
										menuItem.push_back(item);
									}
								}
							}
								break;
							case cocos2d::ui::Widget::TouchEventType::ENDED:
								break;
							default:
								break;
							}
						});
						menuItemShop.at(i)->getButton()->setPosition(Vec2(posWidthShop, posHeightShop));
						posWidthShop += menuItemShop.at(i)->getButton()->getContentSize().width * 0.85;
						menuItemShop.at(i)->setPrePosition(menuItemShop.at(i)->getButton()->getPosition());
						if ((i + 1) % 4 == 0 && i > 0)
						{
							posWidthShop = winSize.width / 2.3;
							posHeightShop -= menuItemShop.at(i)->getButton()->getContentSize().height * 0.9;
						}
					}
				}
				//auto labelAttributeItem = Label::createWithTTF()
				lbl->setPosition(winSize / 2);
				CONFIRM_DIALOGUE_SIZE_OFFSET = Size(CONFIRM_DIALOGUE_SIZE_OFFSET.width, 300);
			}
			node->addChild(lbl, 10);
			node->initBg(lbl->getContentSize() + CONFIRM_DIALOGUE_SIZE_OFFSET, title);
			node->autorelease();
			return node;
		}

		CC_SAFE_DELETE(node);
		return nullptr;
	}
	void PopupShop::initBg(const cocos2d::Size size, const std::string & title)
	{
		Size winSize = Director::getInstance()->getWinSize();

		_bg = ui::ImageView::create(IMAGEPATH::BACKGROUND_IMAGE_SHOP);
		this->addChild(_bg);

		_bg->setPosition(Point(winSize.width / 2, winSize.height / 2));
		_bg->setScale9Enabled(true);
		//_bg->setContentSize(size);
		//_bg->setVisible(false);

		ui::ImageView *fill = ui::ImageView::create(IMAGEPATH::BACKGROUND_IMAGE_SHOP);
		//_bg->addChild(fill);
		fill->setColor(Color3B(210, 210, 210));
		fill->setScale9Enabled(true);
		fill->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		fill->setPosition(Point(FONT::LABEL_OFFSET / 4, FONT::LABEL_OFFSET / 4));
		//fill->setContentSize(Size(size.width - FONT::LABEL_OFFSET / 2, size.height - FONT::LABEL_OFFSET * 2));


		Label *heading = Label::createWithTTF(title, FONT::GAME_FONT, FONT::TITLE_TEXT_SIZE);
		heading->setPosition(_bg->getContentSize().width / 2, _bg->getContentSize().height - FONT::LABEL_OFFSET / 2.3);
		_bg->addChild(heading);
		heading->enableOutline(Color4B::BLACK, FONT::LABEL_STROKE);
		heading->enableShadow(Color4B::BLACK, Size(0, -3));
	}
}