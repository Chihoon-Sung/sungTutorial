#include <stack>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cmath>
using namespace std; 

// 연산자 우선순위
int precedence(char op){ 
	if(op == '+'||op == '-') 
	    return 1; 
	if(op == '*'||op == '/') 
	    return 2; 
	return 0; 
} 

// 연산 적용 후 값 리턴
float applyOp(float a, float b, char op){ 
	switch(op){ 
	case '+': 
        return a + b; 
    case '-': 
        return a - b; 
    case '*': 
        return a * b; 
    case '/':
        if(b == 0)
            throw runtime_error("divide by zero");
        return a / b; 
	} 
} 

// expression tree를 구성 후 처리하는 것이 아닌 stack만으로 처리합니다.
// preProcess 후에 호출합니다.
float evaluate(string tokens){ 
	int i; 	
	stack <float> values; // 실수 값
	stack <char> ops; // 연산자
	
	for(i = 0; i < tokens.length(); i++){
        if(tokens[i] == '('){ // 여는 괄호가 있으면 스택에 추가합니다.
			ops.push(tokens[i]); 
		} 
		else if(isdigit(tokens[i])){ // 현재 문자가 numeric value인지 검사합니다.
			float val = 0; 
			int start = i; // '.' 을 포함한 numeric value의 출발 인덱스입니다.
            string val_str;
			while(i < tokens.length() && (isdigit(tokens[i]) || (tokens[i] == '.'))) // 실수값에 포함되지 않을 문자(괄호나 연산자)가 나올 때까지 이동합니다. 
				i++;
            val_str = tokens.substr(start, i - start); // i 직전 인덱스 까지가 float으로 변환될 문자열 입니다.
			val = stof(val_str); // 문자열을 float으로 변환합니다.
			values.push(val); // 실수 값 스택에 추가합니다.
            i--; // 현재 numeric value 이후 문자를 가리키고 있으므로 인덱스를 하나 감소시킵니다.
		} 
		else if(tokens[i] == ')') // 닫는 괄호가 나오면
		{   
			while(!ops.empty() && ops.top() != '(') // 연산자가 비어있지 않고, 스택의 최상위 원소가 여는 괄호가 아니면 다음 반복문을 수행합니다.
			{ 
                // 스택에서 두 수를 꺼내고 연산자를 꺼내 연산 값을 값 스택에 다시 넣습니다.
				float val2 = values.top(); 
				values.pop(); 
				float val1 = values.top(); 
				values.pop(); 
				char op = ops.top(); 
				ops.pop(); 
				values.push(applyOp(val1, val2, op)); 
			}
			if(!ops.empty()) 
			    ops.pop();
            
		} 
		else // ()나 numeric value가 아니면 ==> 연산자 이면 아래를 수행합니다.
		{ 
			while(!ops.empty() && precedence(ops.top()) >= precedence(tokens[i])){ // 연산자 스택이 비어있지 않고, 연산자 우선순위가 스택 최상위 원소보다 낮거나 같으면 아래를 수행합니다.
                // 더 높은 우선순위의 연산자를 처리해버리고
				float val2 = values.top(); 
				values.pop();				
				float val1 = values.top(); 
				values.pop();				
				char op = ops.top(); 
				ops.pop();				
				values.push(applyOp(val1, val2, op)); 
			} 
            // 현재 연산자를 스택에 넣습니다.
			ops.push(tokens[i]); 
		} 
	} 
	
    // 남아있는 스택에 대한 연산을 수행합니다.
	while(!ops.empty()){ 
		float val2 = values.top(); 
		values.pop();				
		float val1 = values.top(); 
		values.pop();				
		char op = ops.top();
		ops.pop();				
		values.push(applyOp(val1, val2, op)); 
	} 
	return values.top(); 
} 

// balance를 테스트합니다.
bool balanceTest(string raw){
    stack<char> s; 
    char x; 
    for (int i=0; i<raw.length(); i++) 
    {   
        // 여는 괄호는 다 스택에 담아줍니다.
        if (raw[i] == '(' || raw[i] == '[' || raw[i] == '{') 
        {
            s.push(raw[i]); 
            continue; 
        }else if(raw[i] == ')' || raw[i] == ']' || raw[i] == '}'){
            // 닫는 괄호가 나왔는데 스택이 비어있으면 false를 리턴합니다.
            if(s.empty())
                return false;
            
            // 스택 최상위 문자가 각 닫는 괄호에 상응하는 여는 괄호가 아니면 false를 반환합니다.
            switch (raw[i]) 
            { 
            case ')':
                x = s.top(); 
                if (x=='{' || x=='[') 
                    return false; 
                s.pop();
                break;   
            case '}':
                x = s.top(); 
                if (x=='(' || x=='[') 
                    return false; 
                s.pop(); 
                break;
            case ']': 
                x = s.top(); 
                if (x =='(' || x == '{') 
                    return false; 
                s.pop(); 
                break; 
            } 
        }
    }
    // 모든 문자열 검사가 끝난 후 스택이 비어있으면 (모든 괄호가 쌍을 알맞게 찾고 마무리 되면) empty 입니다.
    return (s.empty()); 
}

// balance test를 거친 스트링만을 입력으로 받습니다.
// [], {}, () 를 모두 ()로 치환합니다.
string preProcess(string raw){
    string processed = "";
    for(int i = 0; i < raw.length(); i++){
        if(raw[i] == '{' || raw[i] == '[')
            raw[i] = '(';
        else if(raw[i] == '}' || raw[i] == ']')
            raw[i] = ')';
        processed += raw[i];
    }
    return processed;
}

int main() { 
    string infix="";
    float result;

    while(infix != "EOI"){
        cin >> infix;
        
        if(infix != "EOI"){
            if(balanceTest(infix)){
                infix = preProcess(infix);
                try{
                    result = evaluate(infix);
                    result = round(result * 1000) / 1000; // precision
                    cout << result << endl;
                }catch (runtime_error& e){
                    cout << "Error!: "  << e.what() << endl;
                }
            }else{
                cout << "Error!: unbalanced parantheses" << endl;
            }
        }
    }
	return 0; 
}