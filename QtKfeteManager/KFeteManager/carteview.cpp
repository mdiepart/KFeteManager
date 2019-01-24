#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QButtonGroup>
#include <QPushButton>
#include <QMap>
#include <QDebug>
#include <QToolButton>
#include <QCompleter>
#include <QLineEdit>

#include "carteview.h"
#include "cartemodel.h"

CarteView::CarteView(QWidget *parent, bool inUse) : QWidget(parent)
{
    //Checks the number of pages
    if(CarteModel::PAGES_NAMES.size() != CarteModel::NB_MENU_PAGES){
        throw std::out_of_range("From class Carte : NB_MENU_PAGES not less or equal PAGES_NAMES.size()");
    }

    QVBoxLayout *vBox       = new QVBoxLayout(this); //Main Layout
    QHBoxLayout *hBox       = new QHBoxLayout(); //Layout for page pages buttons
    QStackedLayout *sLayout = new QStackedLayout(); //main layout with pages inside
    QButtonGroup *tabs      = new QButtonGroup(this);
    carteButtons            = new QButtonGroup(this);
    this->inUse = inUse;
    hBox->setSpacing(0);
    vBox->setSpacing(0);

    //set a size policy for the buttons to expand at max
    QSizePolicy qsp(QSizePolicy::Expanding, QSizePolicy::Expanding, QSizePolicy::PushButton);

    QPushButton *tmp;
    for(int i = 0; i < static_cast<int>(CarteModel::NB_MENU_PAGES); i++){
        QWidget *page = new QWidget(this);
        QGridLayout *gridLayout = new QGridLayout(page);
        gridLayout->setSpacing(0);
        //Fills pages
        for(int h = 0; h < static_cast<int>(CarteModel::GRID_H); h++){
            for(int w = 0; w < static_cast<int>(CarteModel::GRID_W); w++){
                //w+(h*GRID_W)+(GRID_H*GRID_W*i)
                //Generating buttons for the carte
                tmp = new QPushButton(page);
                tmp->setSizePolicy(qsp);
                carteButtons->addButton(tmp, w + (h*static_cast<int>(CarteModel::GRID_W)) + (static_cast<int>(CarteModel::GRID_H)*static_cast<int>(CarteModel::GRID_W)*i));
                gridLayout->addWidget(tmp, static_cast<int>(h), static_cast<int>(w));
            }
        }
        //insert pages in QStackedLayout
        sLayout->addWidget(page);

        //Generating tabs
        //Insert buttons in buttonGroup for tabs
        tmp = new QPushButton(CarteModel::PAGES_NAMES.at(i), this);
        tmp->setMinimumHeight(25);
        tmp->setMaximumHeight(80);
        tmp->setSizePolicy(qsp);
        tmp->setCheckable(true);
        hBox->addWidget(tmp, Qt::AlignCenter);
        tabs->addButton(tmp, i);
    }

    if(inUse){
        //Inserts the search button
        search = new QToolButton(this);
        search->setText(tr("&Rechercher"));
        search->setPopupMode(QToolButton::InstantPopup);
        search->setArrowType(Qt::NoArrow);
        search->setShortcut(Qt::CTRL+Qt::Key_F);
        search->setMinimumHeight(25);
        search->setMaximumHeight(80);
        search->setSizePolicy(qsp);
        hBox->addWidget(search);
    }
    vBox->addLayout(sLayout, 8);
    vBox->addLayout(hBox, 1);

    tabs->setExclusive(true);

    connect(tabs, SIGNAL(buttonClicked(int)), sLayout, SLOT(setCurrentIndex(int)));
    tabs->button(0)->click();
}

void CarteView::updateButton(unsigned int id){
    auto *details = model->getButton(id);

    if(details == nullptr){
        //disables button;
        QAbstractButton *button = carteButtons->button(static_cast<int>(id));
        QString ss;
        button->setStyleSheet("");
        button->setText("");
        if(inUse){
            carteButtons->button(static_cast<int>(id))->setDisabled(true);
        }
    }else{
        QAbstractButton *button = carteButtons->button(static_cast<int>(id));
        button->setDisabled(false);
        QString ss;
        ss.append("color: ");
        ss.append(details->getTextColor().name());
        ss.append("; background-color: ");
        ss.append(details->getBackgroundColor().name());
        ss.append("; border: none;");
        button->setStyleSheet(ss);
        button->setText(details->getName());
    }
    delete details;
}

void CarteView::updateView(){
    for(unsigned int i = 0; i < CarteModel::NB_MENU_PAGES*CarteModel::GRID_H*CarteModel::GRID_W; i++){
        updateButton(i);
    }
    if(inUse){
        searcher->refreshList(model->getArticlesList());
    }
}

void CarteView::setModel(CarteModel *model){
    this->model = model;
    connect(carteButtons, SIGNAL(buttonClicked(int)), model, SLOT(buttonClicked(int)) );
    connect(model, SIGNAL(modelUpdated()), this, SLOT(updateView()));
    if(inUse){
        searcher = new Searcher(model->getArticlesList(), this);
        search->addAction(searcher);
        connect(searcher, SIGNAL(articleSearched(QString)), model, SIGNAL(articleClicked(QString)));
    }
    updateView();
}

Searcher::Searcher(const QStringList *list, QWidget *parent) : QWidgetAction (parent){
    this->entriesList = list;
}

Searcher::~Searcher(){
    delete entriesList;
}

void Searcher::refreshList(QStringList *list){
    delete this->entriesList;
    this->entriesList = list;
}

QWidget *Searcher::createWidget(QWidget *parent){
    QCompleter *completer = new QCompleter(*entriesList, parent);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    lineEdit = new QLineEdit(parent);
    lineEdit->setCompleter(completer);
    lineEdit->setFocus(Qt::MenuBarFocusReason);
    connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(returnPressed()));
    lastCreatedWidget = parent;
    return lineEdit;
}

void Searcher::returnPressed(){
    emit articleSearched(lineEdit->text());
    lastCreatedWidget->hide();
}
