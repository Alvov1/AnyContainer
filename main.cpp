struct ICloneable
{
    virtual ICloneable* clone() const = 0;
    virtual ~ICloneable() = default;
};

template <typename Type>
struct ValueHolder : ICloneable{
    explicit ValueHolder(const Type & data) : data_(data) {};

    ValueHolder * clone() const override {
        return new ValueHolder(*this);
    };

    Type data_;
};

class Any
{
public:
    Any() : ptr(nullptr){};

    template <class ValueType>
    explicit Any(const ValueType & param)
            : ptr(new ValueHolder<ValueType>(param)){};

    ~Any(){
        delete ptr;
    }

    Any(const Any & any){
        if(any.ptr)
            ptr = any.ptr->clone();
    };

    Any& operator=(const Any& any) {
        if(this != &any){
            delete ptr;
            ptr = any.ptr->clone();
        }
        return *this;
    }

    template<typename ValueType>
    Any & operator=(const ValueType & value){
        delete ptr;
        ptr = new ValueHolder<ValueType>(value);
        return *this;
    };

    template<typename ValueType>
    ValueType* cast(){
        auto* ptr_ = dynamic_cast<ValueHolder<ValueType>*>(ptr);
        if(ptr_)
            return &(ptr_->data_);
        else
            return nullptr;
    };

private:
    ICloneable* ptr;
};