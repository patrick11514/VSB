namespace ProductsApp
{
    internal class Calculator
    {
        private int x;
        private int y;

        public event EventHandler OnSetXY;
        public event EventHandler<CalcArgs> OnCompute;

        public void SetXY(int x, int y)
        {
            this.x = x;
            this.y = y;

            OnSetXY?.Invoke(this, EventArgs.Empty);
        }

        public void Execute(Operation op) 
        {
            int result = op(this.x, this.y);
            Console.WriteLine(result);
            OnCompute?.Invoke(this, new CalcArgs(result));
        }
    }
}