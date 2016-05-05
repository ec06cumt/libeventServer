USE [uu278]
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [tbluAccount]
(
	--[ID] [int] IDENTITY(1,1) NOT NULL,											
	[UserID] [int] IDENTITY(1,1) NOT NULL,											--用户ID
	[UserName] [varchar](32) COLLATE Chinese_PRC_CI_AS NOT NULL,					--用户名
	[PassWord] [varchar](32) COLLATE Chinese_PRC_CI_AS NOT NULL,					--用户密码
	--[GLevel] [tinyint] NOT NULL CONSTRAINT [DF_tbluGuild_GLevel] DEFAULT(1),		--家族等级，默认1级
	[CreateTime] [datetime] NULL CONSTRAINT [DF_tbluAccount_CreateTime] DEFAULT(getdate()),--家族创建时间默认当前时间
	--[UpdateTime] [datetime] NULL,													--家族信息更新时间
	--[bRefuseJoin] [tinyint] NOT NULL CONSTRAINT [DF_tbluGuild_GRefuse] DEFAULT(0),	--是否拒绝玩家加入家族
	CONSTRAINT [PK_tbluAccount] PRIMARY KEY CLUSTERED									--家族主键
	(
		[UserID]
	)WITH FILLFACTOR = 90 ON [PRIMARY]												--设置填充因子,加快读取速度
) ON [PRIMARY]

SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER ON
---------------------------------------------------------------------------创建存储过程---------------------------------------

/****** Object:  StoredProcedure [dbo].[GSP_GG_AccountInfo_GetInfo]    Script Date: 2015/8/28 9:53:49 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[GSP_GG_AccountInfo_GetInfo]
	@iUserID int 				--玩家唯一标识
AS
	SET NOCOUNT ON
	
	--家族名称是否已经被用
	IF NOT EXISTS(SELECT UserID FROM tbluAccount WHERE UserID = @iUserID)
	BEGIN 
		RETURN -1
	END

	IF @iUserID < 0 
	BEGIN
		RETURN -2
	END
	

	SELECT UserID,UserName,PassWord,CreateTime FROM tbluAccount WHERE UserID = @iUserID

	return 0
SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER ON


/****** Object:  StoredProcedure [dbo].[GSP_GG_AccountInfo_GetInfoByUserName]    Script Date: 2015/8/28 9:51:39 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[GSP_GG_AccountInfo_GetInfoByUserName]
	@strUserName varchar(25) 				--玩家唯一标识
AS
	SET NOCOUNT ON
	
	--家族名称是否已经被用
	IF NOT EXISTS(SELECT UserID FROM tbluAccount WHERE UserName = @strUserName)
	BEGIN 
		RETURN -1
	END

	SELECT UserID,UserName,PassWord,CreateTime FROM tbluAccount WHERE UserName = @strUserName

	return 0
SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER ON


SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[GSP_GG_Add_Account]
	@iUserID int OUTPUT,				--玩家唯一标识
	@szUserName varchar(32),
	@szPassWord varchar(32)
AS
	SET NOCOUNT ON
	BEGIN TRAN
	
	IF EXISTS(SELECT @iUserID FROM tbluAccount WHERE UserID = @iUserID)
	BEGIN 
		ROLLBACK TRAN
		RETURN -1
	END

	IF @iUserID < 0 
	BEGIN
		ROLLBACK TRAN
		RETURN -2
	END
	
	SET @iUserID = @@IDENTITY

	INSERT INTO tbluAccount(UserName,PassWord,CreateTime) VALUES (@szUserName,@szPassWord,GetDate());
	

	COMMIT TRAN

SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER ON