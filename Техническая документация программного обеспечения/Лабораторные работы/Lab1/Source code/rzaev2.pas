program rzaev2;
var
   list : array [1..10] of Integer;
   i, min : Integer;
begin
     writeln('������� 10 �����');
     for i := 1 to 10 do
         read(list[i]);
     min := list[1];
     for i := 2 to 10 do
         if list[i] < min then
            min := list[i];
     writeln('����������� �� 10 �����: ', min);
end.
