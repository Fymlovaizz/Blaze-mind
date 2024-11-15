------------------------------------------
|   Ý Tưởng Trò Chơi Mô Phỏng Tiến Hóa   |
------------------------------------------

Mục Tiêu:
- Mô phỏng quá trình tiến hóa và chọn lọc tự nhiên ở quy mô vi trùng.
- Khám phá cách thế giới trở nên đa dạng sinh học từ một tổ tiên chung duy nhất.
- Tìm hiểu cách một loài trở nên vượt trội, trong khi các loài khác có thể không tiến xa hơn.
- Liệu loài mạnh mẽ hay loài thích ứng tốt với môi trường sẽ chiến thắng?

Các Yếu Tố Trong Trò Chơi:
-----------------------
1. **Vi Trùng**:
   - Phát triển, nhân bản và sinh tồn trong môi trường động.
   - Mục tiêu: Duy trì sự sống sót và thích nghi của loài.

2. **Môi Trường**:
   - Độ ẩm, nhiệt độ, cường độ ánh sáng (RGB), và tài nguyên (chủ yếu là rêu).
   
3. **Đặc Tính Của Vi Trùng**:
   - **Màu Sắc**:
     - Màu sắc càng xa giá trị RGB của môi trường thì vi trùng càng phát triển mạnh.
   - **Màu Mắt**:
     - Ảnh hưởng đến khả năng nhìn (sáng hay tối).
     - Mắt càng tối giúp nhìn rõ vật thể, nhưng tối quá có thể giảm tuổi thọ mắt (mất dần độ nhạy theo thời gian).
   - **Kích Thước**:
     - Kích thước lớn mang lại lợi thế trong giao tranh, nhưng tốn nhiều năng lượng hơn.
   - **Tốc Độ Di Chuyển Trung Bình**.
   - **Tốc Độ Tức Thời**:
     - Tốc độ khi truy bắt hoặc đớp mồi.
   - **Tốc Độ Xoay**:
     - Tốc độ xoay trung bình và tức thời.
   - **Độ Bền Cơ Bắp**.
   - **Sức Mạnh Chiến Đấu**:
     - Chịu ảnh hưởng bởi kích thước; vi trùng lớn có sức mạnh chiến đấu lớn.

4. **Sinh Sản**:
   - **Lượng Vitamin Cần Thiết**:
     - Max 100 (lượng vitamin cao sẽ cải thiện sức khỏe của mẹ và con).
     - Lượng vitamin thấp làm giảm sức khỏe của mẹ và con sau khi nhân bản.
   - **Chuyển Hóa Năng Lượng**:
     - Tốc độ tiêu hóa và khả năng lưu trữ, thải năng lượng.

5. **Độc Tố**:
   - **Các Loại Độc Tố**: A', B', C':
     - A' làm giảm khả năng hấp thụ chất.
     - B' giảm sinh lực nhưng cường độ giảm theo thời gian.
     - C' giảm khả năng chống chịu môi trường.
   - **Khả Năng Kháng Độc**:
     - Vi trùng có thể phát triển khả năng kháng độc.
     - Các độc tố có chứa dinh dưỡng nhưng ở cường độ thấp.

6. **Năng Lượng**:
   - **Lưu Trữ Năng Lượng**.
   - **Tiêu Tốn Năng Lượng**.
   - Vi trùng sẽ chết nếu sức khỏe hoặc năng lượng cạn kiệt.

7. **Cơ Chế Nhân Bản**:
   - **Vitamin C**: Cần thiết để đạt đến mức phát triển tối đa và tự nhân bản.
   - Vi trùng sẽ nhân bản khi đủ điều kiện (có đủ Vitamin C).

8. **Tiến Hóa và Thích Ứng**:
   - Vi trùng tiến hóa theo thời gian dựa trên môi trường và các đặc tính di truyền.
   - Khả năng thích ứng quyết định sự sống sót, sinh sản và thống trị.

9. **Sự Phát Triển Của Rêu**:
   - Rêu tự động nhân bản sau một khoảng thời gian nhất định.
   - Quá trình nhân bản phụ thuộc vào điều kiện ánh sáng.
   - Điều kiện phát triển tối ưu: RGB (155, 155, 155), Nhiệt độ: 35°C, Độ ẩm: 35%.

10. **Giao Diện Môi Trường**:
   - Ba cột RGB hiển thị điều kiện ánh sáng của môi trường.
   - Nhiệt độ và độ ẩm hiển thị dưới cột RGB.

11. **Tác Động Của Môi Trường**:
   - Các yếu tố môi trường ảnh hưởng trực tiếp đến sự phát triển của vi trùng và rêu.
   - Tương tác giữa ánh sáng, nhiệt độ và độ ẩm quyết định sự sống sót và sinh sản.

Mục Tiêu Trò Chơi:
- Khám phá cách các sinh vật thích ứng với các yếu tố môi trường và phát triển đa dạng qua thời gian.
- Sự sống sót được quyết định bởi khả năng cân bằng năng lượng, sức khỏe và khả năng thích nghi với môi trường.

-----------------------
Chi tiết code:
1. **Cơ chế di chuyển**:
- Các sinh vật sẽ đứng im trong trạng thái bình thường, đôi khi chúng sẽ di chuyển một cách ngẫu nhiên và có thể chạm vào 1 thứ nào đó.
-----------------------
Sinh vật:
1. Mùn: Các chất hữu cơ đã phân hủy từ các sinh vật đã chết hoặc chất thải của chúng;
   - Di chuyển theo quán tính.
2. Rêu: Các tế bào rêu;
   - Di chuyển theo quán tính
   - Dinh dưỡng:
      + Phụ thuộc vào ánh sáng, các chất khoáng
   - Bài tiết: CO2, ...
3. Trùng giày (Paramecium):
   - Hình dạng: khối, không đối xứng
   - Kích thước: 50-350 micromet
   - Di chuyển:
      + Dùng các lông bơi
      + Có thể đạt tới 500 micromet/giây
   - Dinh dưỡng: 
      + Vi khuẩn (chưa có);
      + Vụn hữu cơ
   - Bài tiết: CO2, NH3, Nước và các Ion;
   - Sinh sản: 
      + Vô tính
      + Hữu tính: 
         * Hai cá thể trùng giày tiếp xúc
         * Trao đổi chất liệu di chuyền
         * Kết hợp nhân, tạo ra nhân mới mang đặc tính di truyền của cả 2 cá thể
         * Mỗi cá thể trùng giày sẽ tách ra và tiếp tục phân chia theo phương pháp phân đôi
4. Trùng diệp lục (Euglena):
   - Hình dạng:
   - Kích thước: 35-55 micromet
   - Di chuyển:
      + Roi
      + 120 micromet/giây