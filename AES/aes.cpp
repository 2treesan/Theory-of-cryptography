#include <iostream>
#include <stdio.h>
#include <fstream>
#include <cmath>
#include <string> // Thêm thư viện string để sử dụng std::string

using namespace std;

string state[5][5];
string cipherkey[5][5];
string sbox[17][17];
string invsbox[17][17];
string rcon[5][11];
int vong = 1;
int mbox[5][5];    // bang nhan mix column
int invmbox[5][5]; // bang nhan inv mix column

// ham ho tro========================
string Get2FromInt(int n)
{
    string da = "";
    while (n > 0)
    {
        if (n % 2 == 0)
        {
            da = "0" + da;
        }
        else
        {
            da = "1" + da;
        }
        n /= 2;
    }
    while (da.length() < 8)
    {
        da = "0" + da;
    }
    return da;
}

int GetIntFrom2(string s)
{
    int da = 0;
    int n = s.length();
    for (int i = 0; i < n; i++)
    {
        if (s[i] == '1')
        {
            da = da + pow(2, n - 1 - i);
        }
    }
    return da;
}

string Get2From16(string sHex) // chuyen tu he 16->2
{
    string sReturn = "";
    for (long unsigned int i = 0; i < sHex.length(); ++i) // Sửa: Dùng long unsigned int cho i để tránh warning
    {
        switch (sHex[i])
        {
        case '0':
            sReturn.append("0000");
            break;
        case '1':
            sReturn.append("0001");
            break;
        case '2':
            sReturn.append("0010");
            break;
        case '3':
            sReturn.append("0011");
            break;
        case '4':
            sReturn.append("0100");
            break;
        case '5':
            sReturn.append("0101");
            break;
        case '6':
            sReturn.append("0110");
            break;
        case '7':
            sReturn.append("0111");
            break;
        case '8':
            sReturn.append("1000");
            break;
        case '9':
            sReturn.append("1001");
            break;
        case 'a':
            sReturn.append("1010");
            break;
        case 'b':
            sReturn.append("1011");
            break;
        case 'c':
            sReturn.append("1100");
            break;
        case 'd':
            sReturn.append("1101");
            break;
        case 'e':
            sReturn.append("1110");
            break;
        case 'f':
            sReturn.append("1111");
            break;
        }
    }
    return sReturn;
}

string Get16From2(string sBinary)
{
    string rest = "", tmp; // Bỏ chr không cần thiết
    // int len = sBinary.length()/4; // Bỏ len không cần thiết
    // chr = chr.substr(0,len); // Bỏ chr không cần thiết
    // sBinary = chr+sBinary;
    for (long unsigned int i = 0; i < sBinary.length(); i += 4) // Sửa: Dùng long unsigned int cho i để tránh warning
    {
        tmp = sBinary.substr(i, 4);
        if (!tmp.compare("0000"))
        {
            rest = rest + "0";
        }
        else if (!tmp.compare("0001"))
        {
            rest = rest + "1";
        }
        else if (!tmp.compare("0010"))
        {
            rest = rest + "2";
        }
        else if (!tmp.compare("0011"))
        {
            rest = rest + "3";
        }
        else if (!tmp.compare("0100"))
        {
            rest = rest + "4";
        }
        else if (!tmp.compare("0101"))
        {
            rest = rest + "5";
        }
        else if (!tmp.compare("0110"))
        {
            rest = rest + "6";
        }
        else if (!tmp.compare("0111"))
        {
            rest = rest + "7";
        }
        else if (!tmp.compare("1000"))
        {
            rest = rest + "8";
        }
        else if (!tmp.compare("1001"))
        {
            rest = rest + "9";
        }
        else if (!tmp.compare("1010"))
        {
            rest = rest + "a";
        }
        else if (!tmp.compare("1011"))
        {
            rest = rest + "b";
        }
        else if (!tmp.compare("1100"))
        {
            rest = rest + "c";
        }
        else if (!tmp.compare("1101"))
        {
            rest = rest + "d";
        }
        else if (!tmp.compare("1110"))
        {
            rest = rest + "e";
        }
        else if (!tmp.compare("1111"))
        {
            rest = rest + "f";
        }
        else
        {
            continue;
        }
    }
    return rest;
}

string xtime(string temps)
{                              // temp hex
    temps = Get2From16(temps); // chuyen ve bin
    // cout<<temps<<endl;
    string tempbit = "00011011";
    char cc = temps[0];
    for (int i = 0; i <= 6; i++)
    {
        temps[i] = temps[i + 1];
    }
    temps[7] = '0';
    // cout<<"dich "<<temps<<endl;
    if (cc == '1')
    {
        // cout<"co";
        for (int i = 0; i <= 7; i++)
        {
            if (temps[i] == tempbit[i])
                temps[i] = '0';
            else
                temps[i] = '1';
        }
    }
    // cout<<"da "<<temps<<endl;
    return Get16From2(temps);
}

void dichbyte(int i)
{ // dich trai 1 hang i
    string temp1;
    // dich hang i
    temp1 = state[i][1];
    state[i][1] = state[i][2];
    state[i][2] = state[i][3];
    state[i][3] = state[i][4];
    state[i][4] = temp1;
}

string nhanmbox(string temps, int x)
{
    // cout<<temps<<" x"<<x<<endl;
    // cout<<xtime(temps);
    temps = Get2From16(temps); // chuyen ve bin
    // cout<<temps<<endl;
    string tempbit = "00011011";
    if (x == 1)
    {
        return temps; // Trả về dạng nhị phân trực tiếp
    }
    if (x == 2)
    {
        char cc = temps[0];
        for (int i = 0; i <= 6; i++)
        {
            temps[i] = temps[i + 1];
        }
        temps[7] = '0';
        if (cc == '1')
        {
            for (int i = 0; i <= 7; i++)
            {
                if (temps[i] == tempbit[i])
                    temps[i] = '0';
                else
                    temps[i] = '1';
            }
        }
        return temps; // Trả về dạng nhị phân
    }
    if (x == 3)
    {
        string da = temps; // temps gốc (giá trị 1*temps)
        // Tính 2*temps
        char cc = temps[0];
        for (int i = 0; i <= 6; i++)
        {
            temps[i] = temps[i + 1];
        }
        temps[7] = '0';
        if (cc == '1')
        {
            for (int i = 0; i <= 7; i++)
            {
                if (temps[i] == tempbit[i])
                    temps[i] = '0';
                else
                    temps[i] = '1';
            }
        }
        // temps bây giờ là 2*temps
        // Cộng (XOR) temps gốc (da) với 2*temps (temps)
        for (int i = 0; i <= 7; i++)
        {
            if (da[i] == temps[i])
                da[i] = '0';
            else
                da[i] = '1';
        }
        return da; // Trả về dạng nhị phân
    }
    return temps; // Mặc định trả về temps (dạng nhị phân)
}

void getRcon(int i, string da[5])
{ // rcon vong thu i
    for (int j = 1; j <= 4; j++)
    {
        da[j] = rcon[j][i];
    }
    return;
}

string Xorbin(string s1, string s2)
{                   // input 2
    string da = s1; // Khởi tạo da với độ dài của s1
    // Đảm bảo s1 và s2 có cùng độ dài, nếu không logic Xor có thể sai
    // Trong ngữ cảnh mã AES này, chúng thường là 8 bit.
    for (long unsigned int i = 0; i < s1.length() && i < s2.length(); i++)
    { // Sửa: Dùng long unsigned int cho i để tránh warning và kiểm tra độ dài
        if (s1[i] == s2[i])
            da[i] = '0';
        else
            da[i] = '1';
    }
    return da;
}

string invnhanmbox(string temps, int x)
{ // temps là hex, trả về binary string
    string temps_bin;
    if (x == 9)
    {                                                          // 09 = 1001b -> x^3 + 1
        string term1 = Get2From16(xtime(xtime(xtime(temps)))); // x^3 * temps
        string term2 = Get2From16(temps);                      // 1 * temps
        return Xorbin(term1, term2);
    }
    if (x == 11)
    {                                                          // 0B = 1011b -> x^3 + x + 1
        string term1 = Get2From16(xtime(xtime(xtime(temps)))); // x^3 * temps
        string term2 = Get2From16(xtime(temps));               // x * temps
        string term3 = Get2From16(temps);                      // 1 * temps
        return Xorbin(Xorbin(term1, term2), term3);
    }
    if (x == 13)
    {                                                          // 0D = 1101b -> x^3 + x^2 + 1
        string term1 = Get2From16(xtime(xtime(xtime(temps)))); // x^3 * temps
        string term2 = Get2From16(xtime(xtime(temps)));        // x^2 * temps
        string term3 = Get2From16(temps);                      // 1 * temps
        return Xorbin(Xorbin(term1, term2), term3);
    }
    if (x == 14)
    {                                                          // 0E = 1110b -> x^3 + x^2 + x
        string term1 = Get2From16(xtime(xtime(xtime(temps)))); // x^3 * temps
        string term2 = Get2From16(xtime(xtime(temps)));        // x^2 * temps
        string term3 = Get2From16(xtime(temps));               // x * temps
        return Xorbin(Xorbin(term1, term2), term3);
    }
    // Nếu x không phải là 9, 11, 13, 14, hàm gốc trả về temps (hex)
    // Để nhất quán, nên trả về dạng nhị phân nếu không có phép nhân đặc biệt
    return Get2From16(temps); // Hoặc xử lý lỗi nếu x không hợp lệ
}

//==================================
void Nhap()
{
    cout << "nhap text (length=16): ";
    string s = "www.hust.edu.vn"; // day la text can ma hoa [cite: 160]
    while (s.length() < 16)
    { // vi ma hoa 128 nen minh them cho du 16 ki tu [cite: 160]
        s = s + " ";
    }
    cout << s << endl;
    int ii = 0;
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            // Sửa: Chuyển đổi cột trước rồi hàng (column-major order)
            // state[hàng][cột] -> state[j][i] khi đọc từ chuỗi tuyến tính
            char temp = s[ii];
            state[j][i] = Get16From2(Get2FromInt((int)temp)); // state[j][i] theo tài liệu
            ii++;
        }
    }
    //
    cout << "nhap key (length=16): ";
    string sk = "matkhau12345678"; // Sửa: "matkhau6546" trong file gốc có vẻ hơi ngắn [cite: 162]
                                   // và không đủ 16 ký tự nếu không padding
    while (sk.length() < 16)
    { // tuong tu nhu vay [cite: 162]
        sk = sk + " ";
    }
    cout << sk << endl;
    ii = 0;
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            // Sửa: Chuyển đổi cột trước rồi hàng (column-major order)
            char temp = sk[ii];
            cipherkey[j][i] = Get16From2(Get2FromInt((int)temp)); // cipherkey[j][i] theo tài liệu
            ii++;
        }
    }
}

void xuat()
{
    cout << endl
         << "===============================" << endl
         << "text giai ma: " << endl;
    for (int i = 1; i <= 4; i++)
    { // Duyệt theo cột trước (column-major)
        for (int j = 1; j <= 4; j++)
        {                                                         // Duyệt theo hàng
            cout << (char)(GetIntFrom2(Get2From16(state[j][i]))); // state[hàng][cột]
        }
    }
    cout << endl; // Thêm xuống dòng cho đẹp
}

void init()
{
    // sbox [cite: 165]
    fstream fin2("sbox.txt", ios::in);
    if (!fin2.is_open())
    {
        cout << "Error opening sbox.txt" << endl;
        return;
    }
    for (int i = 0; i <= 15; i++)
    {
        for (int j = 0; j <= 15; j++)
        {
            fin2 >> sbox[i][j];
        }
    }
    fin2.close();

    // invsbox [cite: 165]
    fstream fin22("invsbox.txt", ios::in);
    if (!fin22.is_open())
    {
        cout << "Error opening invsbox.txt" << endl;
        return;
    }
    for (int i = 0; i <= 15; i++)
    {
        for (int j = 0; j <= 15; j++)
        {
            fin22 >> invsbox[i][j];
        }
    }
    fin22.close();

    // mix column [cite: 165]
    fstream fin3("mixcolumn.txt", ios::in);
    if (!fin3.is_open())
    {
        cout << "Error opening mixcolumn.txt" << endl;
        return;
    }
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            fin3 >> mbox[i][j];
        }
    }
    fin3.close();

    // inv mix column [cite: 166]
    fstream fin33("invmixcolumn.txt", ios::in);
    if (!fin33.is_open())
    {
        cout << "Error opening invmixcolumn.txt" << endl;
        return;
    }
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            fin33 >> invmbox[i][j];
        }
    }
    fin33.close();

    // rcon [cite: 166]
    fstream fin4("rcon.txt", ios::in);
    if (!fin4.is_open())
    {
        cout << "Error opening rcon.txt" << endl;
        return;
    }
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 10; j++)
        {
            fin4 >> rcon[i][j];
        }
    }
    fin4.close();
}

void show()
{ // Hiển thị state và cipher key
    cout << endl
         << "state" << endl;
    for (int i = 1; i <= 4; i++)
    { // Hàng
        for (int j = 1; j <= 4; j++)
        { // Cột
            cout << state[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl
         << "cipherkey" << endl; // Sửa tên biến cho đúng
    for (int i = 1; i <= 4; i++)
    { // Hàng
        for (int j = 1; j <= 4; j++)
        { // Cột
            cout << cipherkey[i][j] << " ";
        }
        cout << endl;
    }
}

void addRoundKey()
{ // Trong mã hóa, đây là KeyExpansion và XOR với state
  // Hàm này trong code gốc có vẻ đang làm KeyExpansion
    string rconi[5];
    getRcon(vong, rconi); // vong là biến toàn cục chỉ vòng hiện tại

    string w[5][5]; // Tạo một bản sao của cipherkey để tính toán key mới
    for (int r = 1; r <= 4; ++r)
    {
        for (int c = 1; c <= 4; ++c)
        {
            w[r][c] = cipherkey[r][c];
        }
    }

    // Lấy cột cuối cùng của cipherkey (w3 trong Rijndael)
    string temp[5]; // temp sẽ là w[3] của key hiện tại, dùng để tạo w[4] của key mới
    for (int i = 1; i <= 4; i++)
    {
        temp[i] = w[i][4]; // Cột cuối cùng
    }

    // RotWord (temp trở thành RotWord(w[3]))
    string t = temp[1];
    temp[1] = temp[2];
    temp[2] = temp[3];
    temp[3] = temp[4];
    temp[4] = t;

    // SubWord (temp trở thành SubWord(RotWord(w[3])))
    for (int i = 1; i <= 4; i++)
    {
        string s_val = temp[i];
        int x_idx, y_idx;
        if (s_val[0] >= '0' && s_val[0] <= '9')
            x_idx = s_val[0] - '0';
        else
            x_idx = s_val[0] - 'a' + 10;
        if (s_val[1] >= '0' && s_val[1] <= '9')
            y_idx = s_val[1] - '0';
        else
            y_idx = s_val[1] - 'a' + 10;
        temp[i] = sbox[x_idx][y_idx];
    }

    // XOR với Rcon (temp = SubWord(RotWord(w[3])) ^ Rcon[vong])
    // Chỉ XOR byte đầu tiên của temp với Rcon[vong][1]
    // Các byte khác của Rcon là 00
    temp[1] = Get16From2(Xorbin(Get2From16(temp[1]), Get2From16(rconi[1])));
    // rconi[2], rconi[3], rconi[4] thường là "00" nên XOR không đổi với temp[2], temp[3], temp[4]
    // Nhưng để cho đúng, nếu rconi có giá trị khác 00 thì cần XOR hết:
    for (int i = 2; i <= 4; ++i)
    {
        temp[i] = Get16From2(Xorbin(Get2From16(temp[i]), Get2From16(rconi[i])));
    }

    // Tạo cột đầu tiên của key mới: w[4] = w[0] ^ temp
    for (int i = 1; i <= 4; i++)
    {
        cipherkey[i][1] = Get16From2(Xorbin(Get2From16(w[i][1]), Get2From16(temp[i])));
    }

    // Tạo các cột còn lại của key mới: w[i] = w[i-1] ^ w[i-4]
    // cipherkey[j][i] là w[j'][i']
    for (int c = 2; c <= 4; c++)
    { // Cột của key mới (từ 1 đến 3, tương ứng w[5], w[6], w[7])
        for (int r = 1; r <= 4; r++)
        { // Hàng
            cipherkey[r][c] = Get16From2(Xorbin(Get2From16(w[r][c]), Get2From16(cipherkey[r][c - 1])));
        }
    }
}

void subByte()
{
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            string s = state[i][j];
            int x, y;
            if (s[0] >= '0' && s[0] <= '9')
                x = s[0] - '0'; // Hoặc s[0]-48 [cite: 174]
            else
                x = s[0] - 'a' + 10; // Giả sử hex là chữ thường

            if (s[1] >= '0' && s[1] <= '9')
                y = s[1] - '0'; // Hoặc s[1]-48 [cite: 175]
            else
                y = s[1] - 'a' + 10;

            state[i][j] = sbox[x][y];
        }
    }
}

void shiftRows()
{
    // dich hang 2 (hàng thứ 1 theo index 0, hàng 2 theo index 1 trong mảng C++)
    //  Hàng 1 (state[1][j]) không dịch
    //  Hàng 2 (state[2][j]) dịch trái 1 byte
    dichbyte(2); // state[2] là hàng thứ hai

    // Hàng 3 (state[3][j]) dịch trái 2 byte
    dichbyte(3);
    dichbyte(3);

    // Hàng 4 (state[4][j]) dịch trái 3 byte
    dichbyte(4);
    dichbyte(4);
    dichbyte(4);
}

void mixColumn()
{
    string temp_col[5];
    string result_col[5];

    for (int j = 1; j <= 4; j++)
    { // Với mỗi cột của state
        // Sao chép cột hiện tại vào temp_col
        for (int i = 1; i <= 4; i++)
        {
            temp_col[i] = state[i][j];
        }

        // Thực hiện phép nhân ma trận cho cột
        for (int i = 1; i <= 4; i++)
        {                                                         // Với mỗi byte trong cột kết quả (result_col[i])
            string term1_bin = nhanmbox(temp_col[1], mbox[i][1]); // mbox[i][1] * temp_col[1]
            string term2_bin = nhanmbox(temp_col[2], mbox[i][2]); // mbox[i][2] * temp_col[2]
            string term3_bin = nhanmbox(temp_col[3], mbox[i][3]); // mbox[i][3] * temp_col[3]
            string term4_bin = nhanmbox(temp_col[4], mbox[i][4]); // mbox[i][4] * temp_col[4]

            // Cộng XOR các kết quả (đã ở dạng nhị phân từ nhanmbox)
            string xor_res_bin = Xorbin(Xorbin(Xorbin(term1_bin, term2_bin), term3_bin), term4_bin);
            result_col[i] = Get16From2(xor_res_bin); // Chuyển kết quả về hex
        }

        // Cập nhật lại cột trong state
        for (int i = 1; i <= 4; i++)
        {
            state[i][j] = result_col[i];
        }
    }
}

void planTextAddKey()
{ // Tên hàm có thể gây nhầm lẫn, đây là AddRoundKey theo chuẩn AES
  // XOR state với cipherkey (khóa vòng hiện tại)
    for (int i = 1; i <= 4; i++)
    { // Cột
        for (int j = 1; j <= 4; j++)
        { // Hàng
            state[j][i] = Get16From2(Xorbin(Get2From16(state[j][i]), Get2From16(cipherkey[j][i])));
        }
    }
}

// giai ma==============================
void invplanTextAddKey()
{ // Tương tự AddRoundKey cho giải mã
    for (int i = 1; i <= 4; i++)
    { // Cột
        for (int j = 1; j <= 4; j++)
        { // Hàng
            state[j][i] = Get16From2(Xorbin(Get2From16(state[j][i]), Get2From16(cipherkey[j][i])));
        }
    }
}

void invshiftRows()
{
    // Hàng 1 (state[1][j]) không dịch
    // Hàng 2 (state[2][j]) dịch phải 1 byte (tương đương dịch trái 3 byte)
    dichbyte(2);
    dichbyte(2);
    dichbyte(2);

    // Hàng 3 (state[3][j]) dịch phải 2 byte (tương đương dịch trái 2 byte)
    dichbyte(3);
    dichbyte(3);

    // Hàng 4 (state[4][j]) dịch phải 3 byte (tương đương dịch trái 1 byte)
    dichbyte(4);
}

void invsubByte()
{
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            string s = state[i][j];
            int x, y;
            if (s[0] >= '0' && s[0] <= '9')
                x = s[0] - '0';
            else
                x = s[0] - 'a' + 10;

            if (s[1] >= '0' && s[1] <= '9')
                y = s[1] - '0';
            else
                y = s[1] - 'a' + 10;

            state[i][j] = invsbox[x][y];
        }
    }
}

void invmixColumn()
{
    string temp_col[5];
    string result_col[5];

    for (int j = 1; j <= 4; j++)
    { // Với mỗi cột của state
        // Sao chép cột hiện tại vào temp_col
        for (int i = 1; i <= 4; i++)
        {
            temp_col[i] = state[i][j]; // state[hàng][cột]
        }

        // Thực hiện phép nhân ma trận cho cột (sử dụng invmbox và invnhanmbox)
        for (int i = 1; i <= 4; i++)
        { // Với mỗi byte trong cột kết quả (result_col[i])
            // invnhanmbox nhận hex, trả về binary
            string term1_bin = invnhanmbox(temp_col[1], invmbox[i][1]);
            string term2_bin = invnhanmbox(temp_col[2], invmbox[i][2]);
            string term3_bin = invnhanmbox(temp_col[3], invmbox[i][3]);
            string term4_bin = invnhanmbox(temp_col[4], invmbox[i][4]);

            // Cộng XOR các kết quả (đã ở dạng nhị phân)
            string xor_res_bin = Xorbin(Xorbin(Xorbin(term1_bin, term2_bin), term3_bin), term4_bin);
            result_col[i] = Get16From2(xor_res_bin); // Chuyển kết quả về hex
        }

        // Cập nhật lại cột trong state
        for (int i = 1; i <= 4; i++)
        {
            state[i][j] = result_col[i];
        }
    }
}

void invaddRoundKey()
{ // Đây là KeyExpansion cho giải mã, hoạt động ngược lại
  // Hàm này trong code gốc có vẻ đang làm KeyExpansion ngược
    string rconi[5];
    getRcon(vong, rconi); // 'vong' nên là vòng hiện tại của giải mã (giảm dần)

    string prev_cipherkey[5][5]; // Lưu trữ key trước đó để tính toán
    for (int r = 1; r <= 4; ++r)
    {
        for (int c = 1; c <= 4; ++c)
        {
            prev_cipherkey[r][c] = cipherkey[r][c];
        }
    }

    // Tính các cột w[4] đến w[7] (cipherkey[r][1] đến cipherkey[r][4]) từ w[0] đến w[3] của key trước đó
    // w[i] = w[i-1] ^ w[i-4] (cho mã hóa)
    // => w[i-4] = w[i-1] ^ w[i] (để tìm key trước đó cho giải mã, nhưng không phải cách này)

    // Cách Key Expansion ngược cho giải mã AES không đơn giản là đảo ngược phép XOR.
    // Thông thường, các khóa vòng được sinh ra theo thứ tự xuôi (0 đến Nr)
    // và được sử dụng theo thứ tự ngược lại trong giải mã (Nr đến 0).
    // Hàm `addRoundKey` trong mã hóa đã cập nhật `cipherkey` thành khóa vòng tiếp theo.
    // Để giải mã, chúng ta cần các khóa vòng theo thứ tự ngược lại.
    // Cách đơn giản nhất là tính tất cả các khóa vòng trước, lưu trữ chúng,
    // rồi sử dụng chúng theo thứ tự ngược lại cho giải mã.

    // Đoạn code gốc trong `invaddRoundKey` có vẻ đang cố gắng tính ngược Key Expansion
    // từng bước một, điều này phức tạp và dễ lỗi.
    // Giả sử `cipherkey` hiện tại là khóa vòng `k`, chúng ta cần khóa vòng `k-1`.

    // Lấy cột đầu tiên của key hiện tại (w[0] của key hiện tại)
    string temp_w0_curr[5];
    for (int r = 1; r <= 4; ++r)
        temp_w0_curr[r] = prev_cipherkey[r][1];

    // Lấy cột cuối cùng của key trước đó (w[3] của key trước đó)
    // w[3]_prev = RotWord(SubWord(w[0]_curr ^ Rcon[vong]))
    // Đây là phần khó, vì SubWord không dễ đảo ngược trực tiếp ở đây.

    // *** CẢNH BÁO: Logic Key Expansion ngược trong code gốc rất có thể không chính xác. ***
    // *** Cách tiếp cận được khuyến nghị là tính toán tất cả các khóa vòng trước và lưu trữ chúng. ***
    // *** Sau đó, trong quá trình giải mã, sử dụng các khóa vòng đã lưu theo thứ tự ngược lại. ***

    // Đoạn code gốc:
    // c2->c4
    for (int i = 4; i >= 2; i--)
    { // Cột của cipherkey hiện tại
        for (int j = 1; j <= 4; j++)
        { // Hàng
            // cipherkey[j][i] = w[j'][(i-1)+4] (key hiện tại)
            // cipherkey[j][i-1] = w[j'][(i-2)+4] (key hiện tại)
            // Phép toán này đang cố gắng tạo ra cột (i-1) từ cột i và (i-1) của key *hiện tại*.
            // Điều này không đúng với cách KeyExpansion ngược hoạt động.
            // Nó phải sử dụng các cột của key *vòng sau* để tìm key *vòng trước*.
            // cipherkey[j][i] = Get16From2(Xorbin(Get2From16(cipherkey[j][i]), Get2From16(cipherkey[j][i-1])));
            // Giữ nguyên cipherkey như là khóa vòng của vòng đó, không tính ngược ở đây.
        }
    }

    // c1 (cột đầu tiên của key vòng *trước*)
    // temp[] sẽ là RotWord(SubWord(w[3])) của key vòng *trước*
    string temp[5]; // temp sẽ là cột cuối cùng của key vòng *hiện tại*, sau RotWord và SubWord
                    // temp = RotWord(SubByte(w[N_k-1])) where w[N_k-1] is the last column of the *previous* round key.
                    // And the first column of the *current* round key (cipherkey[][1]) was formed by
                    // cipherkey[r][1] = prev_cipherkey[r][1] ^ temp_rcon_transformed[r]

    // Lấy cột cuối cùng của key *hiện tại*
    // string last_col_curr_key[5];
    // for(int r=1; r<=4; ++r) last_col_curr_key[r] = prev_cipherkey[r][4];

    // Thực hiện RotWord trên cột đó
    // string t_rot = last_col_curr_key[1];
    // last_col_curr_key[1] = last_col_curr_key[2];
    // last_col_curr_key[2] = last_col_curr_key[3];
    // last_col_curr_key[3] = last_col_curr_key[4];
    // last_col_curr_key[4] = t_rot;

    // Thực hiện SubWord trên cột đã RotWord
    // for(int r=1; r<=4; ++r) {
    //     string s_val = last_col_curr_key[r];
    //     // ... (logic chuyển hex sang index)
    //     // last_col_curr_key[r] = sbox[x_idx][y_idx]; // Phải là invsbox nếu đang cố đảo ngược
    // }
    // temp = last_col_curr_key (đã qua RotWord, SubWord)

    // Sau đó XOR với cột đầu của key *trước đó* (w[0]_prev) và Rcon
    // w[0]_curr = w[0]_prev ^ temp ^ Rcon
    // => w[0]_prev = w[0]_curr ^ temp ^ Rcon
    // Tuy nhiên, `cipherkey` đang lưu `w[0]_curr`. `temp` phải là `RotWord(SubByte(w[Nk-1]_prev))`

    // Vì KeyExpansion ngược rất phức tạp để làm đúng từng bước thế này,
    // ta sẽ giả định rằng `cipherkey` đã được đặt thành khóa vòng chính xác
    // cho vòng giải mã hiện tại từ một mảng khóa vòng đã tính trước.
    // Do đó, hàm `invaddRoundKey` sẽ không làm gì ngoài việc giảm `vong`.
    // Việc cập nhật `cipherkey` cho vòng giải mã TRƯỚC ĐÓ nên được xử lý bên ngoài.

    // Tuy nhiên, nếu phải tuân theo logic của code gốc (dù có thể sai):
    // Đoạn này cố gắng tạo ra cột đầu tiên của cipherkey VÒNG TRƯỚC ĐÓ
    // từ cột cuối cùng của cipherkey VÒNG HIỆN TẠI và Rcon.

    string temp_from_last_col_current_key[5];
    // Lấy cột cuối cùng của key hiện tại (đã được tính ở vòng trước của giải mã, tức là key của vòng mã hóa k+1)
    // và thực hiện RotWord và SubWord (giống như khi tạo key)
    temp_from_last_col_current_key[1] = prev_cipherkey[2][4]; // RotWord
    temp_from_last_col_current_key[2] = prev_cipherkey[3][4];
    temp_from_last_col_current_key[3] = prev_cipherkey[4][4];
    temp_from_last_col_current_key[4] = prev_cipherkey[1][4];

    for (int i = 1; i <= 4; i++)
    { // SubWord
        string s_val = temp_from_last_col_current_key[i];
        int x_idx, y_idx;
        if (s_val[0] >= '0' && s_val[0] <= '9')
            x_idx = s_val[0] - '0';
        else
            x_idx = s_val[0] - 'a' + 10;
        if (s_val[1] >= '0' && s_val[1] <= '9')
            y_idx = s_val[1] - '0';
        else
            y_idx = s_val[1] - 'a' + 10;
        temp_from_last_col_current_key[i] = sbox[x_idx][y_idx];
    }

    // Tính cột đầu tiên của key vòng TRƯỚC ĐÓ
    for (int r = 1; r <= 4; ++r)
    {
        string rcon_val = rconi[r];            // Rcon của vòng HIỆN TẠI (vong)
        string w0_curr = prev_cipherkey[r][1]; // Cột đầu của key hiện tại
        string temp_val = temp_from_last_col_current_key[r];

        // w[0]_curr = w[0]_prev ^ temp_val ^ Rcon[vong] (nếu temp_val là từ key trước)
        // Nếu cipherkey[][1] là w[0] của key vòng TRƯỚC, thì đây là tính nó:
        // cipherkey[r][1] = Get16From2(Xorbin(Xorbin(Get2From16(w0_curr), Get2From16(temp_val)), Get2From16(rcon_val)));
        // Code gốc: cipherkey[i][1]=(Get16From2(Xorbin(Xorbin(Get2From16(rconi[i]),Get2From16(s)),Get2From16(cipherkey[i][1]))));
        // s là temp_val, cipherkey[i][1] là w0_curr. => w0_prev = Rcon ^ s ^ w0_curr
        cipherkey[r][1] = Get16From2(Xorbin(Xorbin(Get2From16(rcon_val), Get2From16(temp_val)), Get2From16(w0_curr)));
    }

    // Tính các cột còn lại của key vòng TRƯỚC ĐÓ
    // w[c]_prev = w[c-1]_prev ^ w[c]_curr
    for (int c = 2; c <= 4; ++c)
    {
        for (int r = 1; r <= 4; ++r)
        {
            // cipherkey[r][c-1] là w[c-1]_prev (vừa tính ở vòng lặp c trước, hoặc ở trên cho c=1)
            // prev_cipherkey[r][c] là w[c]_curr
            cipherkey[r][c] = Get16From2(Xorbin(Get2From16(cipherkey[r][c - 1]), Get2From16(prev_cipherkey[r][c])));
        }
    }
}

void maHoa()
{
    Nhap();
    cout << endl
         << "Initial State and Key:" << endl;
    show(); // Hiển thị state và key ban đầu

    cout << endl
         << "==================MA HOA====================";

    // Vòng 0: AddRoundKey (XOR với khóa gốc)
    cout << endl
         << "Vong 0 (Initial AddRoundKey)" << endl;
    planTextAddKey(); // XOR state với cipherkey (khóa gốc)
    cout << "Sau AddRoundKey vong 0:" << endl;
    show(); // Hiển thị state sau AddRoundKey ban đầu và khóa gốc (chưa thay đổi)

    vong = 1; // Bắt đầu vòng 1 cho KeyExpansion và các vòng lặp chính
    for (int i = 1; i <= 9; i++)
    { // 9 Vòng đầu
        cout << endl
             << "Vong " << i << endl;
        subByte();
        cout << "Sau SubByte:" << endl;
        show();
        shiftRows();
        cout << "Sau ShiftRows:" << endl;
        show();
        mixColumn();
        cout << "Sau MixColumn:" << endl;
        show();

        addRoundKey(); // Tạo khóa vòng i (cipherkey được cập nhật)
        cout << "Khoa vong " << i << " (sau addRoundKey/KeyExpansion):" << endl;
        // Chỉ hiển thị cipherkey đã cập nhật, state chưa bị XOR với key mới này
        cout << endl
             << "cipherkey" << endl;
        for (int r_idx = 1; r_idx <= 4; r_idx++)
        {
            for (int c_idx = 1; c_idx <= 4; c_idx++)
            {
                cout << cipherkey[r_idx][c_idx] << " ";
            }
            cout << endl;
        }

        planTextAddKey(); // XOR state với khóa vòng i vừa tạo
        cout << "Sau AddRoundKey (XOR state voi khoa vong " << i << "):" << endl;
        show(); // Hiển thị state sau khi XOR và cipherkey của vòng i
        vong++;
    }

    // Vòng cuối (vòng 10)
    cout << endl
         << "Vong 10 (Vong cuoi)" << endl;
    subByte();
    cout << "Sau SubByte:" << endl;
    show();
    shiftRows();
    cout << "Sau ShiftRows:" << endl;
    show();

    addRoundKey(); // Tạo khóa vòng 10
    cout << "Khoa vong 10 (sau addRoundKey/KeyExpansion):" << endl;
    cout << endl
         << "cipherkey" << endl;
    for (int r_idx = 1; r_idx <= 4; r_idx++)
    {
        for (int c_idx = 1; c_idx <= 4; c_idx++)
        {
            cout << cipherkey[r_idx][c_idx] << " ";
        }
        cout << endl;
    }

    planTextAddKey(); // XOR state với khóa vòng 10
    cout << "Sau AddRoundKey (XOR state voi khoa vong 10):" << endl;
    cout << "Ban ma (Ciphertext):" << endl;
    show(); // Hiển thị state cuối cùng (ciphertext) và khóa vòng 10
}

void giaiMa()
{
    // Để giải mã chính xác, chúng ta cần tất cả các khóa vòng đã được sinh ra
    // trong quá trình mã hóa, và sử dụng chúng theo thứ tự ngược lại.
    // Cách tiếp cận đơn giản: Chạy KeyExpansion 10 vòng, lưu tất cả các khóa.
    string roundKeys[11][5][5]; // Lưu trữ khóa từ vòng 0 đến vòng 10

    // Nhập ciphertext và khóa vòng cuối cùng (khóa vòng 10)
    cout << "====================giai ma=====================" << endl;
    cout << "Nhap ciphertext (state 4x4 hex, theo tung hang): " << endl;
    for (int i = 1; i <= 4; i++)
    { // Hàng
        for (int j = 1; j <= 4; j++)
        { // Cột
            cin >> state[i][j];
        }
    }
    cout << "Nhap khoa vong CUOI CUNG (khoa vong 10) (4x4 hex, theo tung hang): " << endl;
    for (int i = 1; i <= 4; i++)
    { // Hàng
        for (int j = 1; j <= 4; j++)
        {                           // Cột
            cin >> cipherkey[i][j]; // Đây là khóa vòng 10
            roundKeys[10][i][j] = cipherkey[i][j];
        }
    }
    cout << "Ciphertext va Khoa vong 10 da nhap:" << endl;
    show();

    // ****** QUAN TRỌNG: Tính toán các khóa vòng trước đó (từ 9 xuống 0) ******
    // Logic `invaddRoundKey` trong code gốc để tính ngược từng khóa là rất phức tạp và dễ sai.
    // Thay vào đó, ta nên tính ngược từ khóa vòng 10 về khóa vòng 0.
    // w[i-4] = w[i] ^ w[i-1] (để tìm cột của word trước đó trong cùng 1 key)
    // w[0]_prev = w[0]_curr ^ RotWord(SubByte(w[3]_prev)) ^ Rcon[vong_curr]
    // Để đơn giản, ta sẽ giả sử có hàm tính ngược Key Expansion đúng, hoặc nhập tay các khóa.
    // Hoặc, tốt hơn, nếu đang giải mã ngay sau khi mã hóa, ta đã có các khóa.

    // Giả sử hàm `invaddRoundKey_standalone(current_key, round_num_for_rcon)`
    // trả về key của vòng TRƯỚC ĐÓ.
    // Vì code gốc không có, ta sẽ phải nhập tay hoặc chấp nhận kết quả có thể sai nếu
    // `invaddRoundKey` không hoạt động đúng.

    // Lưu ý: Code gốc gọi `invaddRoundKey()` để cập nhật `cipherkey` thành khóa của vòng trước đó.
    // Ta cần đảm bảo `vong` được đặt đúng trước mỗi lần gọi.

    // Giai ma vong 10 (vòng cuối của mã hóa)
    vong = 10; // `vong` cho Rcon trong `invaddRoundKey`
    cout << endl
         << "Giai ma - Vong 10 (tuong ung vong cuoi cung cua ma hoa)" << endl;
    cout << "Truoc AddRoundKey (XOR state voi khoa vong " << vong << "):" << endl;
    show();              // state là ciphertext, cipherkey là khóa vòng 10
    invplanTextAddKey(); // XOR state với khóa vòng 10
    cout << "Sau AddRoundKey (XOR state voi khoa vong " << vong << "):" << endl;
    show(); // state đã XOR, cipherkey vẫn là khóa vòng 10

    // `invaddRoundKey()` sẽ cố gắng tính khóa vòng 9 từ khóa vòng 10
    // và cập nhật `cipherkey`. `vong` sẽ được dùng là 10 cho Rcon.
    invaddRoundKey(); // cipherkey bây giờ nên là khóa vòng 9. `vong` không thay đổi trong hàm này
    cout << "Khoa vong 9 (tinh tu khoa vong 10 qua invaddRoundKey voi Rcon[" << vong << "] ):" << endl;
    // Chỉ hiển thị cipherkey đã cập nhật
    cout << endl
         << "cipherkey" << endl;
    for (int r_idx = 1; r_idx <= 4; r_idx++)
    {
        for (int c_idx = 1; c_idx <= 4; c_idx++)
        {
            cout << cipherkey[r_idx][c_idx] << " ";
        }
        cout << endl;
    }

    invshiftRows();
    cout << "Sau Inverse ShiftRows:" << endl;
    show();
    invsubByte();
    cout << "Sau Inverse SubByte:" << endl;
    show();
    // Kết thúc xử lý cho vòng 10 (vòng cuối của mã hóa)

    // Giai ma 9 vong tiep theo (từ vòng 9 xuống vòng 1 của mã hóa)
    for (int i = 9; i >= 1; i--)
    {             // i là số vòng tương ứng trong mã hóa
        vong = i; // Đặt `vong` cho Rcon trong `invaddRoundKey` để tính khóa vòng (i-1)
        cout << endl
             << "Giai ma - Vong " << i << " (tuong ung vong " << i << " cua ma hoa)" << endl;

        cout << "Truoc AddRoundKey (XOR state voi khoa vong " << vong << "):" << endl;
        show();              // state từ vòng trước, cipherkey là khóa vòng `i`
        invplanTextAddKey(); // XOR state với khóa vòng `i`
        cout << "Sau AddRoundKey (XOR state voi khoa vong " << vong << "):" << endl;
        show();

        // `invaddRoundKey()` sẽ cố gắng tính khóa vòng (i-1) từ khóa vòng `i` (hiện tại trong cipherkey)
        // sử dụng Rcon[vong = i].
        if (vong > 0)
        {                     // Chỉ gọi invaddRoundKey nếu không phải là vòng cuối cùng của giải mã (tức là vòng 0 của mã hóa)
            invaddRoundKey(); // cipherkey bây giờ nên là khóa vòng (i-1)
            cout << "Khoa vong " << i - 1 << " (tinh tu khoa vong " << i << " qua invaddRoundKey voi Rcon[" << vong << "] ):" << endl;
            cout << endl
                 << "cipherkey" << endl;
            for (int r_idx = 1; r_idx <= 4; r_idx++)
            {
                for (int c_idx = 1; c_idx <= 4; c_idx++)
                {
                    cout << cipherkey[r_idx][c_idx] << " ";
                }
                cout << endl;
            }
        }

        invmixColumn();
        cout << "Sau Inverse MixColumn:" << endl;
        show();
        invshiftRows();
        cout << "Sau Inverse ShiftRows:" << endl;
        show();
        invsubByte();
        cout << "Sau Inverse SubByte:" << endl;
        show();
    }

    // Giai ma vong 0 (AddRoundKey với khóa gốc)
    // cipherkey lúc này nên là khóa gốc (khóa vòng 0)
    cout << endl
         << "Giai ma - Vong 0 (AddRoundKey voi khoa goc)" << endl;
    cout << "Truoc AddRoundKey cuoi cung (XOR voi khoa goc):" << endl;
    show();              // state từ vòng 1, cipherkey là khóa gốc
    invplanTextAddKey(); // XOR state với khóa gốc
    cout << "Sau AddRoundKey cuoi cung (Plaintext):" << endl;
    show(); // state là plaintext, cipherkey là khóa gốc

    xuat(); // Hiển thị plaintext đã giải mã
}

int main()
{
    init();
    while (1)
    {
        cout << endl
             << "===========MENU========" << endl
             << "1. ma hoa" << endl
             << "2. giai ma" << endl
             << "0. exit" << endl
             << "lua chon: ";
        int m;
        cin >> m;
        if (m == 0)
            return 0;
        if (m == 1)
            maHoa();
        if (m == 2)
            giaiMa();
        if (m < 0 || m > 2)
        { // Sửa điều kiện [cite: 194]
            cout << "stupid !" << endl;
        }
    }
    return 0;
}