#include <iostream>
#include <memory>
#include <vector>
#include <array>
#include <random>
#include <algorithm>

template<const char sep = ' ', typename ...Args>
void print(Args && ...args){
    ((std::cout << args << sep) , ...) << "\n"; //expands to (Pack1 , (Pack2 , (Pack3 , Pack4)))
}
template<typename T>
struct NodeList{
    T * values;
    int length;
    auto begin() {return values;}
    auto end() {return values + length;}
    NodeList(){return;}
    NodeList(int _length): length(_length){
        values = new T[length]{0};
    }
    ~NodeList(){
        delete(values);
    }
    void set(T *&& _values){
        for (int i = 0; i < length; i++){
            values[i] = _values[i];
        }
    }
};

class Layer{
    const NodeList<float> & inputNodes;
    NodeList<float> OutputNodes;

    NodeList<float> weights;
    NodeList<float> biases;
    inline float Rnd(){
        return static_cast <float> (((rand()) / static_cast <float> (RAND_MAX)) - 0.5);
    }
    inline int GetIndex(int Input, int Output){
        return Input * inputNodes.length + Output;
    }
    inline float clip(float val, int && min, int && max){
        return std::max<float>(min, std::min<float>(max,val));
    }
    public:
    Layer(NodeList<float> * inputNode, int OutputAmount):   
            inputNodes(*inputNode),
            OutputNodes(OutputAmount),
            weights(inputNode->length*OutputAmount),
            biases(inputNode->length){
    }
    void FillRandom(){
        for (auto & ell : weights){
            ell = Rnd();
        }
        for (auto & ell : biases){
            ell = Rnd();
        }
    }
    void Calculate(){
        for (int i = 0; i < OutputNodes.length; i++){
            float sum = 0;
            for (int j = 0; j < inputNodes.length; j++){
                sum += biases.values[j] + weights.values[GetIndex(j,i)] * inputNodes.values[j];
            }
            OutputNodes.values[i] = clip(sum,-1,1);
        }
    }
    NodeList<float> * GetOutput(){
        return &OutputNodes;
    }
    void printData(){
        print("inputs");
        for (int i = 0; i < inputNodes.length; i++){
            std::cout << inputNodes.values[i] << " ";
        }
        print("");
        print("outputs");
        for (auto & ell : OutputNodes){
            std::cout<< ell << " ";
        }
        print("");
    }
    
};


class Neural{
    std::vector<Layer> layers;
    NodeList<float> * output;
    NodeList<float> * init(NodeList<float> * inputs, NodeList<int> arr){
        auto * inp = inputs;
        for (auto ell: arr){
            layers.push_back(Layer(inp, ell));
            inp = layers.back().GetOutput();
        }
        return inp;
    }
    public:
    Neural(NodeList<float> * inputs, NodeList<int> & arr) : output(init(inputs, arr)){}
    void Randomize(){
        for (auto ell: layers){
            ell.FillRandom();
        }
    }
    void calculate(){
        int ind = 1;
        for (auto ell: layers){
            print("layer", ind);
            ell.Calculate();
            ell.printData();
            ind++;
        }
    }
};

int main(){
    srand(time(NULL));
    NodeList<float> inputs(5);
    float arr[] = {0.2,0.35,0.8,0.7,0.2};
    inputs.set(arr);

    NodeList<int> params(3);
    int arr2[] = {5,4,3};
    params.set(arr2);

    Neural network(&inputs, params);
    network.Randomize();
    print("calculating");
    network.calculate();
}
