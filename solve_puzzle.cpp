#include "utils.hpp"
#include "MoveGener.hpp"
#include "MoveClassifier.hpp"
#include "MoveFilter.hpp"
#include "MovePlayer.hpp"
#include "MinMax.hpp"

extern pthread_mutex_t mutex;

void declare()
{
    cout << "---------------------------\n";
    cout << "����������" << endl;
    cout << "p - ��" << endl;
    cout << "e - �˳�����" << endl;
    cout << "---------------------------\n";
    cout << "xС�� d���� 0����10" << endl;
    cout << "---------------------------\n";
}

const int buf_size = 1000;
char screen_buf[buf_size];
void AddBlankSpace()
{
    int i;
    char buf[buf_size];
    for (i = 999; i >= 0; i--)
    {
        buf[i] = screen_buf[i];
        screen_buf[i] = 32;
    }
    for (i = 499; i >= 0; i--)
    {
        screen_buf[i * 2] = buf[i];
    }
}
int Lorder()
{
    declare();
    ENGINE_TYPE = MULTI_THREAD_ENGINE;

    pthread_mutex_init(&mutex, NULL);

    // Quick Test Case
    // string str_lorder_cards = "2 K 9 8 7 7 6 6 5 4";
    // string str_farmer_cards = "���� 2 A Q J J 9 6 4";

    // Slow Test Case
    // string str_lorder_cards = "С�� ���� K 10 10 7 8 8 6 6";
    // string str_farmer_cards = "2 2 A A A 9 9 7 6 6 3 3";

    // Standard Case
    string str_lorder_cards = "";
    string str_farmer_cards = "";

    if (str_lorder_cards == "" && str_farmer_cards == "")
    {
        cout << "������������ƣ�(�����Կո���)" << endl;
        cin.getline(screen_buf, buf_size);
        AddBlankSpace();
        str_lorder_cards = string(screen_buf);
        bool valid_cards = validate_cards(str_lorder_cards);
        while (!valid_cards)
        {
            cout << "�����������������������ƣ�(�����Կո���)" << endl;
            cin.getline(screen_buf, buf_size);
            AddBlankSpace();
            str_lorder_cards = string(screen_buf);
            valid_cards = validate_cards(str_lorder_cards);
        }

        cout << "������ũ����ƣ�(�����Կո���)" << endl;
        cin.getline(screen_buf, buf_size);
        AddBlankSpace();
        str_farmer_cards = string(screen_buf);
        valid_cards = validate_cards(str_farmer_cards);
        while (!valid_cards)
        {
            cout << "�����������������ũ����ƣ�(�����Կո���)" << endl;
            cin.getline(screen_buf, buf_size);
            AddBlankSpace();
            str_farmer_cards = string(screen_buf);
            valid_cards = validate_cards(str_farmer_cards);
        }
    }

    vector<int> lorder_cards = format_input_cards(get_cards_from_input(str_lorder_cards));
    vector<int> farmer_cards = format_input_cards(get_cards_from_input(str_farmer_cards));
    vector<int> last_move({});

    cout << "��ʼ״̬: " << endl;
    cout << "�����ҵ���:";
    print_vector(format_output_cards(lorder_cards));
    cout << "ũ��ҵ���:";
    print_vector(format_output_cards(farmer_cards));
    cout << "��ǰ������:"
         << "����" << endl;
    cout << "---------------------------" << endl;

    vector<int> lorder_move, farmer_move;
    int score = 0;
    pair<int, vector<int>> result = start_engine(farmer_cards, lorder_cards, last_move, LORDER);
    score = result.first;
    lorder_move = result.second;

    if (score == MIN_SCORE)
    {
        cout << "�����ذ�" << endl;
        return 0;
    }

    lorder_cards = get_rest_cards(lorder_cards, lorder_move);
    if (lorder_cards.size() == 0)
    {
        cout << "��������: ";
        print_vector(format_output_cards(lorder_move));
        cout << "����ʤ��!" << endl;
        return 0;
    }

    // Farmer and LandLorder play one by one
    vector<string> str_lorder_move, str_farmer_move;
    vector<vector<int>> proper_moves;

    while (true)
    {
        // Print the Situation after Lorder play a move
        if (lorder_move.size() > 0)
        {
            str_lorder_move = format_output_cards(lorder_move);
        }
        else
        {
            str_lorder_move = {string("Pass!")};
        }

        cout << "�����ҵ���: ";
        print_vector(format_output_cards(lorder_cards));
        cout << "ũ��ҵ���: ";
        print_vector(format_output_cards(farmer_cards));
        cout << "�����ѳ���: ";
        print_vector(str_lorder_move);
        cout << "---------------------------" << endl;

        // Farmer plays a move
        cout << "���ũ�����:" << endl;
        cin.getline(screen_buf, buf_size);
        AddBlankSpace();
        string input_farmer_move = string(screen_buf);

        str_farmer_move = get_cards_from_input(input_farmer_move);
        if (str_farmer_move.size() == 0 ||
            str_farmer_move[0] == "p" || str_farmer_move[0] == "P")
        {
            farmer_move = {};
        }
        else if (str_farmer_move[0] == "e")
        {
            exit(0);
        }
        else
        {
            farmer_move = format_input_cards(str_farmer_move);
        }

        proper_moves = get_proper_moves(farmer_cards, lorder_move);

        // check if farmer_move exists in proper_moves
        bool farmer_move_exists = false;
        if (farmer_move.size() == 0)
        { // pass move
            farmer_move_exists = true;
        }
        else
        {
            sort(farmer_move.begin(), farmer_move.end());
            for (auto move : proper_moves)
            {
                if (move.size() != farmer_move.size())
                    continue;
                sort(move.begin(), move.end());
                bool all_same = true;
                for (int i = 0; i < move.size(); ++i)
                {
                    if (farmer_move[i] != move[i])
                    {
                        all_same = false;
                        break;
                    }
                }
                if (all_same == true)
                    farmer_move_exists = true;
            }
        }

        while (farmer_move_exists == false)
        {
            cout << "���ƴ��������°�ũ�����:" << endl;
            cin.getline(screen_buf, buf_size);
            AddBlankSpace();
            input_farmer_move = string(screen_buf);
            str_farmer_move = get_cards_from_input(input_farmer_move);
            if (str_farmer_move.size() == 0 ||
                str_farmer_move[0] == "p" || str_farmer_move[0] == "P")
            {
                farmer_move = {};
            }
            else if (str_farmer_move[0] == "e")
            {
                exit(0);
            }
            else
            {
                farmer_move = format_input_cards(str_farmer_move);
            }

            proper_moves = get_proper_moves(farmer_cards, lorder_move);

            // check if farmer_move exists in proper_moves
            farmer_move_exists = false;
            if (farmer_move.size() == 0)
            { // pass move
                farmer_move_exists = true;
            }
            else
            {
                sort(farmer_move.begin(), farmer_move.end());
                for (auto move : proper_moves)
                {
                    if (move.size() != farmer_move.size())
                        continue;
                    sort(move.begin(), move.end());
                    bool all_same = true;
                    for (int i = 0; i < move.size(); ++i)
                    {
                        if (farmer_move[i] != move[i])
                        {
                            all_same = false;
                            break;
                        }
                    }
                    if (all_same)
                        farmer_move_exists = true;
                }
            }
        }

        farmer_cards = get_rest_cards(farmer_cards, farmer_move);
        if (farmer_cards.size() == 0)
        {
            cout << "ũ�����: ";
            print_vector(format_output_cards(farmer_move));
            cout << "ũ��ʤ����" << endl;
            return 0;
        }

        if (farmer_move.size() > 0)
        {
            str_farmer_move = format_output_cards(farmer_move);
        }
        else
        {
            str_farmer_move = {"Pass!"};
        }
        cout << "�����ҵ���: ";
        print_vector(format_output_cards(lorder_cards));
        cout << "ũ��ҵ���: ";
        print_vector(format_output_cards(farmer_cards));
        cout << "ũ���ѳ���: ";
        print_vector(str_farmer_move);
        cout << "---------------------------" << endl;

        // LandLorder plays a move
        find_best_move = false; // recovery this global variable
        best_move = {};
        result = start_engine(farmer_cards, lorder_cards, farmer_move, LORDER);
        score = result.first;
        lorder_move = result.second;

        if (score == MIN_SCORE)
        {
            cout << "�����ذܣ�" << endl;
            return 0;
        }

        lorder_cards = get_rest_cards(lorder_cards, lorder_move);
        if (lorder_cards.size() == 0)
        {
            cout << "��������: ";
            print_vector(format_output_cards(lorder_move));
            cout << "����ʤ����" << endl;
            return 0;
        }
    }

    return 0;
}

int Farmer()
{
    declare();
    ENGINE_TYPE = MULTI_THREAD_ENGINE;

    pthread_mutex_init(&mutex, NULL);

    // Quick Test Case
    // string str_lorder_cards = "2 K 9 8 7 7 6 6 5 4";
    // string str_farmer_cards = "���� 2 A Q J J 9 6 4";

    // Slow Test Case
    // string str_lorder_cards = "С�� ���� K 10 10 7 8 8 6 6";
    // string str_farmer_cards = "2 2 A A A 9 9 7 6 6 3 3";

    // Standard Case
    string str_lorder_cards = "";
    string str_farmer_cards = "";

    if (str_lorder_cards == "" && str_farmer_cards == "")
    {
        cout << "������������ƣ�(�����Կո���)" << endl;
        cin.getline(screen_buf, buf_size);
        AddBlankSpace();
        str_lorder_cards = string(screen_buf);
        bool valid_cards = validate_cards(str_lorder_cards);
        while (!valid_cards)
        {
            cout << "�����������������������ƣ�(�����Կո���)" << endl;
            cin.getline(screen_buf, buf_size);
            AddBlankSpace();
            str_lorder_cards = string(screen_buf);
            valid_cards = validate_cards(str_lorder_cards);
        }

        cout << "������ũ����ƣ�(�����Կո���)" << endl;
        cin.getline(screen_buf, buf_size);
        AddBlankSpace();
        str_farmer_cards = string(screen_buf);
        valid_cards = validate_cards(str_farmer_cards);
        while (!valid_cards)
        {
            cout << "�����������������ũ����ƣ�(�����Կո���)" << endl;
            cin.getline(screen_buf, buf_size);
            AddBlankSpace();
            str_farmer_cards = string(screen_buf);
            valid_cards = validate_cards(str_farmer_cards);
        }
    }

    vector<int> lorder_cards = format_input_cards(get_cards_from_input(str_lorder_cards));
    vector<int> farmer_cards = format_input_cards(get_cards_from_input(str_farmer_cards));
    vector<int> last_move({});

    cout << "��ʼ״̬: " << endl;
    cout << "�����ҵ���:";
    print_vector(format_output_cards(lorder_cards));
    cout << "ũ��ҵ���:";
    print_vector(format_output_cards(farmer_cards));
    cout << "��ǰ������:"
         << "ũ��" << endl;
    cout << "---------------------------" << endl;

    cout << "���ũ�����:" << endl;
    vector<int> lorder_move, farmer_move;
    vector<string> str_lorder_move, str_farmer_move;
    vector<vector<int>> proper_moves;
    lorder_move = {};

    cin.getline(screen_buf, buf_size);
    AddBlankSpace();
    string input_farmer_move = string(screen_buf);
    str_farmer_move = get_cards_from_input(input_farmer_move);
    if (str_farmer_move.size() == 0 ||
        str_farmer_move[0] == "p" || str_farmer_move[0] == "P")
    {
        farmer_move = {};
    }
    else if (str_farmer_move[0] == "e")
    {
        exit(0);
    }
    else
    {
        farmer_move = format_input_cards(str_farmer_move);
    }

    proper_moves = get_proper_moves(farmer_cards, lorder_move);

    // check if farmer_move exists in proper_moves
    bool farmer_move_exists = false;
    if (farmer_move.size() == 0)
    { // pass move
        farmer_move_exists = true;
    }
    else
    {
        sort(farmer_move.begin(), farmer_move.end());
        for (auto move : proper_moves)
        {
            if (move.size() != farmer_move.size())
                continue;
            sort(move.begin(), move.end());
            bool all_same = true;
            for (int i = 0; i < move.size(); ++i)
            {
                if (farmer_move[i] != move[i])
                {
                    all_same = false;
                    break;
                }
            }
            if (all_same == true)
                farmer_move_exists = true;
        }
    }

    while (farmer_move_exists == false)
    {
        cout << "���ƴ��������°�ũ�����:" << endl;
        cin.getline(screen_buf, buf_size);
        AddBlankSpace();
        input_farmer_move = string(screen_buf);
        str_farmer_move = get_cards_from_input(input_farmer_move);
        if (str_farmer_move.size() == 0 ||
            str_farmer_move[0] == "p" || str_farmer_move[0] == "P")
        {
            farmer_move = {};
        }
        else if (str_farmer_move[0] == "e")
        {
            exit(0);
        }
        else
        {
            farmer_move = format_input_cards(str_farmer_move);
        }

        proper_moves = get_proper_moves(farmer_cards, lorder_move);

        // check if farmer_move exists in proper_moves
        farmer_move_exists = false;
        if (farmer_move.size() == 0)
        { // pass move
            farmer_move_exists = true;
        }
        else
        {
            sort(farmer_move.begin(), farmer_move.end());
            for (auto move : proper_moves)
            {
                if (move.size() != farmer_move.size())
                    continue;
                sort(move.begin(), move.end());
                bool all_same = true;
                for (int i = 0; i < move.size(); ++i)
                {
                    if (farmer_move[i] != move[i])
                    {
                        all_same = false;
                        break;
                    }
                }
                if (all_same)
                    farmer_move_exists = true;
            }
        }
    }

    farmer_cards = get_rest_cards(farmer_cards, farmer_move);
    int score = 0;
    pair<int, vector<int>> result = start_engine(farmer_cards, lorder_cards, farmer_move, LORDER);
    score = result.first;
    lorder_move = result.second;

    if (score == MIN_SCORE)
    {
        cout << "�����ذ�" << endl;
        return 0;
    }

    lorder_cards = get_rest_cards(lorder_cards, lorder_move);
    if (lorder_cards.size() == 0)
    {
        cout << "��������: ";
        print_vector(format_output_cards(lorder_move));
        cout << "����ʤ��!" << endl;
        return 0;
    }

    // Farmer and LandLorder play one by one

    while (true)
    {
        // Print the Situation after Lorder play a move
        if (lorder_move.size() > 0)
        {
            str_lorder_move = format_output_cards(lorder_move);
        }
        else
        {
            str_lorder_move = {string("Pass!")};
        }

        cout << "�����ҵ���: ";
        print_vector(format_output_cards(lorder_cards));
        cout << "ũ��ҵ���: ";
        print_vector(format_output_cards(farmer_cards));
        cout << "�����ѳ���: ";
        print_vector(str_lorder_move);
        cout << "---------------------------" << endl;

        // Farmer plays a move
        cout << "���ũ�����:" << endl;
        cin.getline(screen_buf, buf_size);
        AddBlankSpace();
        string input_farmer_move = string(screen_buf);

        str_farmer_move = get_cards_from_input(input_farmer_move);
        if (str_farmer_move.size() == 0 ||
            str_farmer_move[0] == "p" || str_farmer_move[0] == "P")
        {
            farmer_move = {};
        }
        else if (str_farmer_move[0] == "e")
        {
            exit(0);
        }
        else
        {
            farmer_move = format_input_cards(str_farmer_move);
        }

        proper_moves = get_proper_moves(farmer_cards, lorder_move);

        // check if farmer_move exists in proper_moves
        bool farmer_move_exists = false;
        if (farmer_move.size() == 0)
        { // pass move
            farmer_move_exists = true;
        }
        else
        {
            sort(farmer_move.begin(), farmer_move.end());
            for (auto move : proper_moves)
            {
                if (move.size() != farmer_move.size())
                    continue;
                sort(move.begin(), move.end());
                bool all_same = true;
                for (int i = 0; i < move.size(); ++i)
                {
                    if (farmer_move[i] != move[i])
                    {
                        all_same = false;
                        break;
                    }
                }
                if (all_same == true)
                    farmer_move_exists = true;
            }
        }

        while (farmer_move_exists == false)
        {
            cout << "���ƴ��������°�ũ�����:" << endl;
            cin.getline(screen_buf, buf_size);
            AddBlankSpace();
            input_farmer_move = string(screen_buf);
            str_farmer_move = get_cards_from_input(input_farmer_move);
            if (str_farmer_move.size() == 0 ||
                str_farmer_move[0] == "p" || str_farmer_move[0] == "P")
            {
                farmer_move = {};
            }
            else if (str_farmer_move[0] == "e")
            {
                exit(0);
            }
            else
            {
                farmer_move = format_input_cards(str_farmer_move);
            }

            proper_moves = get_proper_moves(farmer_cards, lorder_move);

            // check if farmer_move exists in proper_moves
            farmer_move_exists = false;
            if (farmer_move.size() == 0)
            { // pass move
                farmer_move_exists = true;
            }
            else
            {
                sort(farmer_move.begin(), farmer_move.end());
                for (auto move : proper_moves)
                {
                    if (move.size() != farmer_move.size())
                        continue;
                    sort(move.begin(), move.end());
                    bool all_same = true;
                    for (int i = 0; i < move.size(); ++i)
                    {
                        if (farmer_move[i] != move[i])
                        {
                            all_same = false;
                            break;
                        }
                    }
                    if (all_same)
                        farmer_move_exists = true;
                }
            }
        }

        farmer_cards = get_rest_cards(farmer_cards, farmer_move);
        if (farmer_cards.size() == 0)
        {
            cout << "ũ�����: ";
            print_vector(format_output_cards(farmer_move));
            cout << "ũ��ʤ����" << endl;
            return 0;
        }

        if (farmer_move.size() > 0)
        {
            str_farmer_move = format_output_cards(farmer_move);
        }
        else
        {
            str_farmer_move = {"Pass!"};
        }
        cout << "�����ҵ���: ";
        print_vector(format_output_cards(lorder_cards));
        cout << "ũ��ҵ���: ";
        print_vector(format_output_cards(farmer_cards));
        cout << "ũ���ѳ���: ";
        print_vector(str_farmer_move);
        cout << "---------------------------" << endl;

        // LandLorder plays a move
        find_best_move = false; // recovery this global variable
        best_move = {};
        result = start_engine(farmer_cards, lorder_cards, farmer_move, LORDER);
        score = result.first;
        lorder_move = result.second;

        if (score == MIN_SCORE)
        {
            cout << "�����ذܣ�" << endl;
            return 0;
        }

        lorder_cards = get_rest_cards(lorder_cards, lorder_move);
        if (lorder_cards.size() == 0)
        {
            cout << "��������: ";
            print_vector(format_output_cards(lorder_move));
            cout << "����ʤ����" << endl;
            return 0;
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{
    bool LorderFirst = true;
    if (argc >= 2)
    {
        
        if (atoi(argv[1]) == 1)
        {
            LorderFirst = false;
        }
    }
    if (LorderFirst)
    {
        Lorder();
    }
    else
    {
        Farmer();
    }
}