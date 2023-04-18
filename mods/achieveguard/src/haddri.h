

class haddri{
	public:
		void* target=nullptr;
		void* inject=nullptr;
		void* origin=nullptr;

		bool active=false;
		void enable();
		void disable();

		inline haddri(void* tgt,void* inj){
		this->target=tgt;
		this->inject=inj;
		};
};


