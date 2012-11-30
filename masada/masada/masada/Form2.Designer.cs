namespace masada
{
    partial class Form2
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form2));
            this.typingBoxText = new System.Windows.Forms.TextBox();
            this.typingBoxLabel = new System.Windows.Forms.Label();
            this.logoutButton = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // typingBoxText
            // 
            this.typingBoxText.Location = new System.Drawing.Point(68, 256);
            this.typingBoxText.Multiline = true;
            this.typingBoxText.Name = "typingBoxText";
            this.typingBoxText.Size = new System.Drawing.Size(668, 243);
            this.typingBoxText.TabIndex = 0;
            this.typingBoxText.KeyDown += new System.Windows.Forms.KeyEventHandler(this.typingBoxText_KeyDown);
            this.typingBoxText.KeyUp += new System.Windows.Forms.KeyEventHandler(this.typingBoxText_KeyUp);
            // 
            // typingBoxLabel
            // 
            this.typingBoxLabel.AutoSize = true;
            this.typingBoxLabel.Location = new System.Drawing.Point(65, 486);
            this.typingBoxLabel.Name = "typingBoxLabel";
            this.typingBoxLabel.Size = new System.Drawing.Size(0, 13);
            this.typingBoxLabel.TabIndex = 1;
            // 
            // logoutButton
            // 
            this.logoutButton.Location = new System.Drawing.Point(704, 12);
            this.logoutButton.Name = "logoutButton";
            this.logoutButton.Size = new System.Drawing.Size(99, 28);
            this.logoutButton.TabIndex = 2;
            this.logoutButton.Text = "Logout";
            this.logoutButton.UseVisualStyleBackColor = true;
            this.logoutButton.Click += new System.EventHandler(this.logoutButton_Click);
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(113, 65);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(555, 174);
            this.label1.TabIndex = 3;
            this.label1.Text = resources.GetString("label1.Text");
            this.label1.Click += new System.EventHandler(this.label1_Click_1);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(702, 78);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(100, 26);
            this.button1.TabIndex = 4;
            this.button1.Text = "Test Arrays";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.testFilterTimes);
            // 
            // Form2
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(815, 605);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.logoutButton);
            this.Controls.Add(this.typingBoxLabel);
            this.Controls.Add(this.typingBoxText);
            this.Name = "Form2";
            this.Text = "Form2";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form2_FormClosing);
            this.Load += new System.EventHandler(this.Form2_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox typingBoxText;
        private System.Windows.Forms.Label typingBoxLabel;
        private System.Windows.Forms.Button logoutButton;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button button1;
    }
}