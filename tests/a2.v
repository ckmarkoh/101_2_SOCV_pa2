module a (reset, clk,add, z1, z2, z3, z4);
input reset, clk;
output z1, z2, z3, z4;

reg [3:0]x;
reg [3:0]y;

input [3:0] add;
wire [3:0] sum;

//assign z1 = (x == 4'd200);
assign z1 = (x >= 4'd10);
assign z2 = !(x <= 4'd10);
assign z3 = (x > 4'd10);
assign z4 = (x == 4'd10) || (y == 4'd10);

always @(posedge clk) begin
   if (!reset) begin
      x <= 4'd1;
      y <= 4'd1;
   end
   else if(add<4'd2)begin
		x<=x+add;
		y<=y+add;
   end
   else begin
      if (x < 4'd5) begin 
         x <= x + y;
      end
      y <= x;
   end
end
endmodule

